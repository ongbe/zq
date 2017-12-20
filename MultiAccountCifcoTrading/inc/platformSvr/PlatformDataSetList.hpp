/*
    用于维护底层列表类数据，包括报单、成交、持仓明细、持仓、组合持仓
    注意：此类的对象只能在PlatformDataMgr中使用，这里不管加锁
    所有的数据结构体中都有Instrument、FTID、UpdateSeq这几项

    注意
    1. 插入新元素时，先new一个DataStruType，m_all和m_all2都引用new出来的结构体
    2. 删除元素时，需要把DataStruType删掉

    added by l. 20120313
*/


#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/ISvr/BusinessData.hpp"


static long m_baseFTID=1;                                    //FTID基准
static long m_baseUpdateSeq=1;                               //UpdateSeq基准
template <typename KeyType,typename DataStruType>
class CPlatformDataSetList
{
public:
    CPlatformDataSetList()
    {
        m_bNeedsetKeyOfQryRlt=false;

        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2VecOrdOfQryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();
        m_allFTID.clear();


    };

    ~CPlatformDataSetList(void)
    {
        Clear();
    };

    void Clear(void)
    {
        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2VecOrdOfQryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        Clear2();
    }

    void Clear2(void)
    {
        //先记录数据指针
        std::vector<DataStruType*> vecpData;
        vecpData.clear();
        std::map<KeyType,DataStruType*>::iterator it;
        for(it=m_all.begin();it!=m_all.end();it++)
            vecpData.push_back(it->second);

        //清除所有map
        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();
        m_allFTID.clear();

        //释放数据指针内容
        int count=vecpData.size();
        for(int i=0;i<count;i++)
        {
            if(vecpData[i]!=NULL)
            {
                delete vecpData[i];
                vecpData[i]=NULL;
            }
        }
    }

    //bNeedsetKeyOfQryRlt:查询结果返回时，是否需要创建mapQryRlt. 对于持仓，需要创建，用于合并当日持仓和历史持仓
    void SetbNeedsetKeyOfQryRlt(bool bNeedsetKeyOfQryRlt)
    {
        m_bNeedsetKeyOfQryRlt=bNeedsetKeyOfQryRlt;
    }

    //清空查询的map
    void WillQry_Internal(std::string& strQryInstrument)
    {
        m_strQryInstrument=strQryInstrument;
        m_QryRlt.clear();
        m_mapKey2VecOrdOfQryRlt.clear();
        m_InstrumentsInQryRlt.clear();
    };

    //查询结果到来
    void QryRltReach_Internal(const DataStruType& DataInfo,const int ErrID,const bool bIsLast)
    {
        if(ErrID==0)
        {
            std::string strInstrument(DataInfo.InstrumentID);
            m_QryRlt.push_back(DataInfo);
            if(m_bNeedsetKeyOfQryRlt)
            {
                KeyType key(DataInfo);
                if(m_mapKey2VecOrdOfQryRlt.find(key)==m_mapKey2VecOrdOfQryRlt.end())
                    m_mapKey2VecOrdOfQryRlt.insert(make_pair(key,(int)m_QryRlt.size()-1));
            }

            if(m_InstrumentsInQryRlt.find(strInstrument)==m_InstrumentsInQryRlt.end())
                m_InstrumentsInQryRlt.insert(strInstrument);
        }

        if(bIsLast)
        {
            std::map<KeyType,DataStruType*>::iterator it_all;
            std::string strInstrument;
            int i,count;


            //只更新指定合约的数据。为提高效率，分别针对部分更新和全部更新单独处理
            if(!m_strQryInstrument.empty())
            {
                std::map<std::string,std::set<KeyType>>::iterator it;
                std::map<std::string,std::set<long>>::iterator it2;
                std::map<long,KeyType>::iterator it_FTID;
                std::set<long> setFTID;
                long FTID;

                //将m_QryRlt中的数据插入或更新到m_all中
                setFTID.clear();
                count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                    setFTID.insert(m_QryRlt[i].FTID);
                }

                //删除m_all中不再存在的数据(m_QryRlt中没有有的数据)
                //注意，此时setFTID中的数据是有效FTID值，用于决定原有的数据是否需要删除
                it2=m_allIndex2.find(m_strQryInstrument);
                if(it2!=m_allIndex2.end()&&!it2->second.empty())
                {
                    std::set<long>& oldmapFTID=it2->second;
                    std::set<long>::iterator itold=oldmapFTID.begin();
                    std::set<long>::iterator itnew=setFTID.begin();

                    while(itold!=oldmapFTID.end())
                    {
                        FTID=(*itold);
                        itold++;

                        while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                        if(itnew==setFTID.end()||(*itnew)!=FTID)
                        {
                            //itnew里没有对应项目，需要删除
                            DeleteRecord3_Internal(FTID);

                            if(m_allIndex2.find(m_strQryInstrument)==m_allIndex2.end())
                                break;
                        }
                    }
                }
            }
            else
            {
                //更新所有

                //如果原来的Key存在，保留原有的FTID，否则申请新的；同时设置UpdateSeq
                
                //for(i=0;i<count;i++)
                //{
                //    KeyType tmpkey(m_QryRlt[i]);
                //    it_all=m_all.find(tmpkey);
                //    if(it_all!=m_all.end()&&it_all->second!=NULL)
                //        m_QryRlt[i].FTID=it_all->second->FTID;
                //    else 
                //        m_QryRlt[i].FTID=m_baseFTID++;

                //    m_QryRlt[i].UpdateSeq=m_baseUpdateSeq++;
                //}

                //清除原来记录
                Clear2();

                //插入原来记录
				count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                }
            }
        }
    };

    //新数据到来
    void NewReach_Internal(DataStruType& DataInfo)
    {
        DataInfo.UpdateSeq=m_baseUpdateSeq++;

        UpdateRecord_Internal(DataInfo);
    };



    //删除数据
    void DeleteRecord2_Internal(const KeyType& KeyInfo)
    {
        std::map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(KeyInfo);
        if(it_all==m_all.end()||it_all->second==NULL)
            return;

        DataStruType tmpData=*(it_all->second);
        DeleteRecord_Internal(tmpData);
    };

    //删除数据
    void DeleteRecord3_Internal(const long FTID)
    {
        std::map<long,DataStruType*>::iterator it_all2;
        it_all2=m_all2.find(FTID);
        if(it_all2==m_all2.end()||it_all2->second==NULL)
            return;

        DataStruType tmpData=*(it_all2->second);
        DeleteRecord_Internal(tmpData);
    };


    //删除数据
    void DeleteRecord_Internal(const DataStruType& DataInfo)
    {
        long FTID=DataInfo.FTID;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        DataStruType *pData=NULL;

        //删除m_allFTID中相关内容
        std::set<long>::iterator it_FTID;
        it_FTID=m_allFTID.find(FTID);
        if(it_FTID!=m_allFTID.end())
            m_allFTID.erase(it_FTID);

        //删除m_allIndex中相关内容
        std::map<std::string,std::set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&it->second.find(tmpkey)!=it->second.end())
        {
            it->second.erase(tmpkey);
            if(it->second.empty())
                m_allIndex.erase(it);
        }

        //删除m_allIndex2中相关内容
        std::map<std::string,std::set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&it2->second.find(FTID)!=it2->second.end())
        {
            it2->second.erase(FTID);
            if(it2->second.empty())
                m_allIndex2.erase(it2);
        }

        //删除m_all中相关内容
        std::map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            pData=it_all->second;
            m_all.erase(it_all);
        }

        //删除m_all2中相关内容
        std::map<long,DataStruType*>::iterator it_all2;
        it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end()&&it_all2->second!=NULL)
        {
            if(!pData)
                pData=it_all2->second;
            m_all2.erase(it_all2);
        }

        //删除数据内容
        if(pData)
        {
            delete pData;
            pData=NULL;
        }
    };


    ///插入或更新一条新记录
    void UpdateRecord_Internal(DataStruType& DataInfo)
    {
        std::map<KeyType,DataStruType*>::iterator it_all;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        long FTID;

        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            //更新原有数据

            //保留原有的FTID
            FTID=it_all->second->FTID;
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=m_baseUpdateSeq++;

            //更新内容
            *(it_all->second)=DataInfo;
        }
        else
        {
            //插入新数据
        
            //申请新的FTID
            FTID=m_baseFTID++;
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=m_baseUpdateSeq++;

            //new一个新元素
            DataStruType *pvalue=new DataStruType(DataInfo);
            DataStruType& NewData=(*pvalue);

            //插入m_all
            m_all.insert(make_pair(tmpkey,pvalue));

            //插入m_all2
            m_all2.insert(make_pair(FTID,pvalue));

            //插入m_allIndex
            std::map<std::string,std::set<KeyType>>::iterator it;
            it=m_allIndex.find(strInstrument);
            if(it==m_allIndex.end())
            {
                std::set<KeyType> setKey;
                setKey.insert(tmpkey);
                m_allIndex.insert(make_pair(strInstrument,setKey));
            }
            else it->second.insert(tmpkey);

            //插入m_allIndex2
            std::map<std::string,std::set<long>>::iterator it2;
            it2=m_allIndex2.find(strInstrument);
            if(it2==m_allIndex2.end())
            {
                std::set<long> setFTID;
                setFTID.insert(FTID);
                m_allIndex2.insert(make_pair(strInstrument,setFTID));
            }
            else it2->second.insert(FTID);

            //插入m_allFTID
            std::set<long>::iterator it_FTID;
            it_FTID=m_allFTID.find(FTID);
            if(it_FTID==m_allFTID.end())
                m_allFTID.insert(FTID);

        }
    }

    ///获取指定Key的Value，返回成功与否
    bool GetValue_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        std::map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            poutData=it->second;
            return true;
        }
        else
        {
            poutData=NULL;
            return false;
        }
    };


    ///获取指定FTID的Value，返回成功与否
    bool GetValue2_Internal(DataStruType*& poutData,long FTID)
    {
        std::map<long,DataStruType*>::iterator it2;
        it2=m_all2.find(FTID);
        if(it2!=m_all2.end()&&it2->second!=NULL) 
        {
            poutData=it2->second;
            return true;
        }
        else
        {
            poutData=NULL;
            return false;
        }
    };

    ///获取指定FTID的Value，返回成功与否
    bool GetValue3_Internal(DataStruType& outData,long FTID)
    {
        std::map<long,DataStruType*>::iterator it2;
        it2=m_all2.find(FTID);
        if(it2!=m_all2.end()&&it2->second!=NULL) 
        {
            outData=*(it2->second);
            return true;
        }
        else
        {
            return false;
        }
    };


    ///获取m_QryRlt中指定Key的Value的指针，返回成功与否
    bool GetpValueInQryRlt_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        poutData=NULL;

        if(!m_bNeedsetKeyOfQryRlt) 
            return false;

        std::map<KeyType,int>::iterator it;
        
        it=m_mapKey2VecOrdOfQryRlt.find(Key);
        if(it==m_mapKey2VecOrdOfQryRlt.end())
            return false;

        int count=m_QryRlt.size();

        if(it->second>=count) 
            return false;

        KeyType tmpkey(m_QryRlt[it->second]);
        if(tmpkey!=Key)
            return false;

        poutData=&(m_QryRlt[it->second]);

        return true;
    };

    ///获取合约相关的KeySet，返回成功与否
    bool GetKeySetOfInstrument(std::set<KeyType>*& poutKeySet,std::string& strInstrument)
    {
        std::map<std::string,std::set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&!it->second.empty())
        {
            poutKeySet=&(it->second);
            return true;
        }
        else 
        {
            poutKeySet=NULL;
            return false;
        }
    }

    ///获取合约相关的FTIDSet，返回成功与否
    bool GetFTIDSetOfInstrument(std::set<long>*& poutFTIDSet,const std::string& strInstrument)
    {
        std::map<std::string,std::set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            poutFTIDSet=&(it2->second);
            return true;
        }
        else 
        {
            poutFTIDSet=NULL;
            return false;
        }
    }


    ///获取全部FTID
    int GetAllFTID_Internal(std::set<long>& outDatas)
    {
        outDatas=m_allFTID;
    }


    ///获取全部数据，返回数据项的数量
    int GetAll2_Internal(std::vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        std::map<long,DataStruType*>::iterator it;
        
        for(it=m_all2.begin();it!=m_all2.end();it++)
        {
            if(it->second!=NULL)
                outDatas.push_back(*(it->second));
        }
        return outDatas.size();
    };

    ///获取合约相关的全部数据，返回数据项的数量
    int GetAllAboutInstrument2_Internal(const std::string strInstrument,std::vector<DataStruType>& outDatas)
    {
        std::map<std::string,std::set<long>>::iterator it2;

        outDatas.clear();

        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            std::set<long>::iterator itFTID;
            long FTID;
            std::map<long,DataStruType*>::iterator itData;
            for(itFTID=it2->second.begin();itFTID!=it2->second.end();itFTID++)
            {
                FTID=*itFTID;
                itData=m_all2.find(FTID);
                if(itData!=m_all2.end()&&itData->second!=NULL)
                    outDatas.push_back(*(itData->second));
            }
        }
        return outDatas.size();
    };


    ///获取合约相关的全部数据，返回数据项的数量
    int GetAllAboutInstrument_Internal(const std::string strInstrument,std::map<KeyType,DataStruType>& outDatas)
    {
        std::map<std::string,std::set<KeyType>>::iterator itKeyset;
        std::map<KeyType,DataStruType>::iterator it;

        itKeyset=m_allIndex.find(strInstrument);
        outDatas.clear();
        if(itKeyset!=m_allIndex.end()&&!itKeyset->second.empty())
        {
            std::set<KeyType>::iterator itKey;
            for(itKey=itKeyset->second.begin();itKey!=itKeyset->second.end();itKey++)
            {
                it=m_all.find(*itKey);
                if(it!=m_all.end())
                    outDatas.insert(std::make_pair(it->first,it->second));
            }
        }
        return outDatas.size();
    };

    void GetQryInstrument(std::string& outData)
    {
        outData=m_strQryInstrument;
    }

    void GetInstrumentSetOfQryRlt(std::set<std::string>& outData)
    {
        outData=m_InstrumentsInQryRlt;
    }

    long NewUpdateSeq(void)
    {
        long NewSeq=m_baseUpdateSeq++;
        return NewSeq;
    }

    long GetUpdateSeq(void)
    {
        return m_baseUpdateSeq;
    }

    bool HaveDataOfInstrumentID(const std::string& strInstrument)
    {
        std::map<std::string,std::set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        return (it!=m_allIndex.end()&&!it->second.empty()) ? true : false;
    }



    //----------下面是数据集----------
    std::string m_strQryInstrument;                     //查询对应的合约代码，为空时表示查询全部
    std::vector<DataStruType> m_QryRlt;                 //查询的结果
    std::map<KeyType,int> m_mapKey2VecOrdOfQryRlt;      //查询结果的Key to vector order 的map
    std::set<std::string> m_InstrumentsInQryRlt;        //查询结果中的合约列表

    std::map<KeyType,DataStruType*> m_all;              //Key对应的全部记录
    std::map<long,DataStruType*> m_all2;                //FTID对应的全部记录
    std::map<std::string,std::set<KeyType>> m_allIndex; //全部记录的索引, set是按KeyType的定义排序
    std::map<std::string,std::set<long>> m_allIndex2;   //全部记录的索引, set是按FTID的定义排序
    std::set<long> m_allFTID;                           //全部FTID

private:

    bool m_bNeedsetKeyOfQryRlt;
};

