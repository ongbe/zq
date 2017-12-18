/*
    用于维护底层列表类数据，包括报单、成交、持仓明细、持仓、组合持仓明细
    注意：此类的对象只能在PlatformDataMgr中使用，这里不管加锁
    所有的数据结构体中都有Instrument、FTID、UpdateSeq这几项

    注意
    1. 插入新元素时，先new一个DataStruType，m_all和m_all2都引用new出来的结构体
    2. 删除元素时，需要把DataStruType删掉

    added by l. 20120313
*/


#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <set>
#include <string>
using std::string;
using std::map;
using std::vector;
using std::set;
using std::make_pair;

#ifdef WIN32
#pragma managed(push,off)
#endif 


template <typename KeyType,typename DataStruType>
class CPlatformDataSetList
{
public:
    //bUpdateSeq2FTID:是否要维护UpdateSeq和FTID的对应关系。成交和报单要维护，用于增量落地
    CPlatformDataSetList(volatile long& baseFTID,volatile long& baseUpdateSeq,bool bUpdateSeq2FTID=false)
    :   m_bUpdateSeq2FTID(bUpdateSeq2FTID),
        m_baseFTID(baseFTID),
        m_baseUpdateSeq(baseUpdateSeq),
        m_lastUpdateSeq(baseUpdateSeq)
    {
        m_bNeedKey2QryRlt=false;

        m_strQryInstrument.clear();
        m_QryRlt.clear();
        m_mapKey2QryRlt.clear();
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
        m_mapKey2QryRlt.clear();
        m_InstrumentsInQryRlt.clear();

        Clear2();
    }

    void Clear2(void)
    {
        //先记录数据指针
        vector<DataStruType*> vecpData;
        vecpData.clear();
        map<KeyType,DataStruType*>::iterator it;
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

    //bNeedKey2QryRlt:查询结果返回时，是否需要维护m_mapKey2QryRlt. 对于持仓，需要维护，用于合并当日持仓和历史持仓；其它的不用
    void SetbNeedKey2QryRlt(bool bNeedKey2QryRlt)
    {
        m_bNeedKey2QryRlt=bNeedKey2QryRlt;
    }

    //查询前的准备工作，设置要查询的合约名称，清除查询结果缓冲区
    void WillQry_Internal(const string& strQryInstrument)
    {
        m_strQryInstrument=strQryInstrument;
        m_QryRlt.clear();
        m_mapKey2QryRlt.clear();
        m_InstrumentsInQryRlt.clear();
    };

    //查询结果到来
    void QryRltReach_Internal(const DataStruType& DataInfo,const int ErrID,const bool bIsLast)
    {
        if(ErrID==0)
        {
            //先将查询结果放到m_QryRlt中，等全部查完了再统一处理
            string strInstrument(DataInfo.InstrumentID);
            m_QryRlt.push_back(DataInfo);

            if(m_bNeedKey2QryRlt)
            {
                //对于持仓数据，需要维护m_mapKey2QryRlt
                KeyType key(DataInfo);
                if(m_mapKey2QryRlt.find(key)==m_mapKey2QryRlt.end())
                    m_mapKey2QryRlt.insert(make_pair(key,(int)m_QryRlt.size()-1));
            }

            if(m_InstrumentsInQryRlt.find(strInstrument)==m_InstrumentsInQryRlt.end())
                m_InstrumentsInQryRlt.insert(strInstrument);
        }

        if(bIsLast)
        {
            //全部查完了，统一处理暂存在m_QryRlt中的查询结果
            map<KeyType,DataStruType*>::iterator it_all;
            string strInstrument;
            int i,count;


            //只更新指定合约的数据。为提高效率，分别针对部分更新和全部更新单独处理
            if(!m_strQryInstrument.empty())
            {
                map<string,set<KeyType>>::iterator it;
                map<string,set<long>>::iterator it2;
                map<long,KeyType>::iterator it_FTID;
                set<long> setFTID;
                long FTID;

                //将m_QryRlt中的数据插入或更新到m_all中
                setFTID.clear();
                count=m_QryRlt.size();
                for(i=0;i<count;i++)
                {
                    UpdateRecord_Internal(m_QryRlt[i]);
                    setFTID.insert(m_QryRlt[i].FTID);
                }

                //删除m_all中不再存在的数据(m_QryRlt中没有的数据)
                //注意，此时setFTID中的数据是有效FTID值，用于决定原有的数据是否需要删除
                vector<long> InvalidFTIDs;
                it2=m_allIndex2.find(m_strQryInstrument);
                if(it2!=m_allIndex2.end()&&!it2->second.empty())
                {
                    set<long>& oldsetFTID=it2->second;
                    set<long>::iterator itold=oldsetFTID.begin();
                    set<long>::iterator itnew=setFTID.begin();

                    for(itold=oldsetFTID.begin();itold!=oldsetFTID.end();itold++)
                    {
                        FTID=(*itold);
                        while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                        if(itnew==setFTID.end()||(*itnew)!=FTID)
                            InvalidFTIDs.push_back(FTID);
                    }
                }
                //现在InvalidFTIDs中保存了无效的FTID，删除之
                for(int i=0;i<(int)InvalidFTIDs.size();i++)
                    DeleteRecord3_Internal(InvalidFTIDs[i]);
            }
            else
            {
                //更新所有

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
        UpdateRecord_Internal(DataInfo);
    };



    //删除数据，根据指定的Key
    void DeleteRecord2_Internal(const KeyType& KeyInfo)
    {
        map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(KeyInfo);
        if(it_all==m_all.end()||it_all->second==NULL)
            return;

        DataStruType tmpData=*(it_all->second);
        DeleteRecord_Internal(tmpData);
    };

    //删除数据，根据指定的FTID
    void DeleteRecord3_Internal(const long FTID)
    {
        map<long,DataStruType*>::iterator it_all2;
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
        string strInstrument(tmpkey.InstrumentID);
        DataStruType *pData=NULL;

        //删除m_allFTID中相关内容
        set<long>::iterator it_FTID;
        it_FTID=m_allFTID.find(FTID);
        if(it_FTID!=m_allFTID.end())
            m_allFTID.erase(it_FTID);

        //删除m_allIndex中相关内容
        map<string,set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        if(it!=m_allIndex.end()&&it->second.find(tmpkey)!=it->second.end())
        {
            it->second.erase(tmpkey);
            if(it->second.empty())
                m_allIndex.erase(it);
        }

        //删除m_allIndex2中相关内容
        map<string,set<long>>::iterator it2;
        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&it2->second.find(FTID)!=it2->second.end())
        {
            it2->second.erase(FTID);
            if(it2->second.empty())
            {
                //如果合约对应的记录被删除空了，将合约也删掉
                m_allIndex2.erase(it2);
            }
        }

        //删除m_all中相关内容
        map<KeyType,DataStruType*>::iterator it_all;
        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            pData=it_all->second;
            m_all.erase(it_all);
        }

        //删除m_all2中相关内容
        map<long,DataStruType*>::iterator it_all2;
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

            //删除数据后，递增m_baseUpdateSeq，表示内容有变更
            //++m_baseUpdateSeq;
            m_lastUpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;
        }
    };


    ///插入或更新一条新记录
    void UpdateRecord_Internal(DataStruType& DataInfo)
    {
        map<KeyType,DataStruType*>::iterator it_all;
        KeyType tmpkey(DataInfo);
        string strInstrument(tmpkey.InstrumentID);
        long FTID;

        it_all=m_all.find(tmpkey);
        if(it_all!=m_all.end()&&it_all->second!=NULL)
        {
            //更新原有数据

            //保留原有的FTID
            FTID=it_all->second->FTID;
            DataInfo.FTID=FTID;
            //DataInfo.UpdateSeq=m_baseUpdateSeq++;
            DataInfo.UpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            m_lastUpdateSeq=DataInfo.UpdateSeq;
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;

            //更新内容
            *(it_all->second)=DataInfo;
        }
        else
        {
            //插入新数据
        
            //申请新的FTID
            FTID=InterlockedIncrement(&m_baseFTID);
            DataInfo.FTID=FTID;
            DataInfo.UpdateSeq=InterlockedIncrement(&m_baseUpdateSeq);
            m_lastUpdateSeq=DataInfo.UpdateSeq;
            if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;

            //new一个新元素
            DataStruType *pvalue=new DataStruType(DataInfo);

            //插入m_all
            m_all.insert(make_pair(tmpkey,pvalue));

            //插入m_all2
            m_all2.insert(make_pair(FTID,pvalue));

            //插入m_allIndex
            map<string,set<KeyType>>::iterator it;
            it=m_allIndex.find(strInstrument);
            if(it==m_allIndex.end())
            {
                set<KeyType> setKey;
                setKey.insert(tmpkey);
                m_allIndex.insert(make_pair(strInstrument,setKey));
            }
            else it->second.insert(tmpkey);

            //插入m_allIndex2
            map<string,set<long>>::iterator it2;
            it2=m_allIndex2.find(strInstrument);
            if(it2==m_allIndex2.end())
            {
                set<long> setFTID;
                setFTID.insert(FTID);
                m_allIndex2.insert(make_pair(strInstrument,setFTID));
            }
            else it2->second.insert(FTID);

            //插入m_allFTID
            set<long>::iterator it_FTID;
            it_FTID=m_allFTID.find(FTID);
            if(it_FTID==m_allFTID.end())
                m_allFTID.insert(FTID);

        }
    }

    ///获取指定Key的Value，返回成功与否
    ///成功时返回的是内部指针，小心使用
    bool GetValue_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
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
    ///成功时返回的是内部指针，不要修改之。小心使用
    bool GetValue2_Internal(DataStruType*& poutData,long FTID)
    {
        map<long,DataStruType*>::iterator it2;
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
        map<long,DataStruType*>::iterator it2;
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

    ///获取指定Key的Value，返回成功与否
    bool GetValue4_Internal(DataStruType& outData,const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            outData=*(it->second);
            return true;
        }
        else return false;
    };

    ///获取m_QryRlt中指定Key的Value的指针，返回成功与否
    ///成功时返回的是内部指针，不要修改之。小心使用
    ///仅供持仓数据集使用
    bool GetpValueInQryRlt_Internal(DataStruType*& poutData,const KeyType& Key)
    {
        poutData=NULL;

        if(!m_bNeedKey2QryRlt) 
            return false;

        map<KeyType,int>::iterator it;
        
        it=m_mapKey2QryRlt.find(Key);
        if(it==m_mapKey2QryRlt.end())
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
    ///成功时返回的是内部指针，不要修改之。小心使用
    bool GetKeySetOfInstrument(set<KeyType>*& poutKeySet,const string& strInstrument)
    {
        map<string,set<KeyType>>::iterator it;
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
    ///成功时返回的是内部指针，不要修改之。小心使用
    bool GetFTIDSetOfInstrument(set<long>*& poutFTIDSet,const string& strInstrument)
    {
        map<string,set<long>>::iterator it2;
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

    //获取全部FTID，按合约分类。同一个合约的FTID是排序的
    void GetFTIDsByInstrument(map<string,vector<long>>& outFTIDs)
    {
        outFTIDs.clear();
        map<string,set<long>>::const_iterator it;
        for(it=m_allIndex2.begin();it!=m_allIndex2.end();it++)
        {
            if(!it->second.empty())
            {
                outFTIDs[it->first]=vector<long>();
                map<string,vector<long>>::iterator it2=outFTIDs.find(it->first);
                
                for(set<long>::const_iterator it3=it->second.begin();it3!=it->second.end();it3++)
                    it2->second.push_back(*it3);
            }
        }
    }


    ///获取全部FTID
    void GetAllFTID_Internal(set<long>& outDatas)
    {
        outDatas=m_allFTID;
    }

    //获取全部FTID
    void GetAllFTID_Internal(vector<long>& outFTIDs)
    {
        outFTIDs.clear();
        for(set<long>::const_iterator it=m_allFTID.begin();it!=m_allFTID.end();it++)
            outFTIDs.push_back(*it);
    }

    ///获取全部数据，返回数据项的数量。结果放在数组中
    int GetAll2_Internal(vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        map<long,DataStruType*>::iterator it;
        
        for(it=m_all2.begin();it!=m_all2.end();it++)
        {
            if(it->second!=NULL)
                outDatas.push_back(*(it->second));
        }
        return outDatas.size();
    };
    ///获取大于等于MinFTID的全部数据，返回数据项的数量。结果放在数组中。m_bUpdateSeq2FTID为true时有效
    int GetAll_GE_UpdateSeq_Internal(long MinUpdateSeq,vector<DataStruType>& outDatas)
    {
        outDatas.clear();

        if(!m_bUpdateSeq2FTID) return 0;

        set<long> FTIDs;
        for(map<long,long>::const_iterator it=m_UpdateSeq2FTID.lower_bound(MinUpdateSeq);it!=m_UpdateSeq2FTID.end();it++)
        {
            if(FTIDs.find(it->second)==FTIDs.end())
                FTIDs.insert(it->second);
        }
        for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
        {
            map<long,DataStruType*>::const_iterator it2=m_all2.find(*it);
            if(it2!=m_all2.end()&&it2->second!=NULL)
            {
                outDatas.push_back(*(it2->second));
            }
        }

        return outDatas.size();
    };

    ///获取合约相关的全部数据，返回数据项的数量。结果数据放在数组中
    int GetAllAboutInstrument2_Internal(const string strInstrument,vector<DataStruType>& outDatas)
    {
        map<string,set<long>>::iterator it2;

        outDatas.clear();

        it2=m_allIndex2.find(strInstrument);
        if(it2!=m_allIndex2.end()&&!it2->second.empty())
        {
            set<long>::iterator itFTID;
            long FTID;
            map<long,DataStruType*>::iterator itData;
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


    ///获取合约相关的全部数据，返回数据项的数量。结果数据放在map中
    int GetAllAboutInstrument_Internal(const string strInstrument,map<KeyType,DataStruType>& outDatas)
    {
        map<string,set<KeyType>>::iterator itKeyset;
        map<KeyType,DataStruType>::iterator it;

        itKeyset=m_allIndex.find(strInstrument);
        outDatas.clear();
        if(itKeyset!=m_allIndex.end()&&!itKeyset->second.empty())
        {
            set<KeyType>::iterator itKey;
            for(itKey=itKeyset->second.begin();itKey!=itKeyset->second.end();itKey++)
            {
                it=m_all.find(*itKey);
                if(it!=m_all.end())
                    outDatas.insert(make_pair(it->first,it->second));
            }
        }
        return outDatas.size();
    };

    ///获取正在进行查询的合约名称
    void GetQryInstrument(string& outData)
    {
        outData=m_strQryInstrument;
    }

    ///获取查询结果的合约集合
    void GetInstrumentSetOfQryRlt(set<string>& outData)
    {
        outData=m_InstrumentsInQryRlt;
    }

    ///递增更新序号，表示数据内容有更新
    long NewUpdateSeq(long FTID)
    {
        //long NewSeq=m_baseUpdateSeq++;
        long NewSeq=InterlockedIncrement(&m_baseUpdateSeq);
        m_lastUpdateSeq=NewSeq;
        if(m_bUpdateSeq2FTID) m_UpdateSeq2FTID[m_lastUpdateSeq]=FTID;
        return NewSeq;
    }

    ///获取当前的更新序号，用于判断数据内容是否有更新
    long GetLastUpdateSeq(void)
    {
        //return m_baseUpdateSeq;
        return m_lastUpdateSeq;
    }

    ///判断是否有指定合约的相关数据
    bool HaveDataOfInstrumentID(const string& strInstrument)
    {
        map<string,set<KeyType>>::iterator it;
        it=m_allIndex.find(strInstrument);
        return (it!=m_allIndex.end()&&!it->second.empty()) ? true : false;
    }
    ///判断指定key的数据项是否存在
    bool IsExist(const KeyType& Key)
    {
        map<KeyType,DataStruType*>::iterator it=m_all.find(Key);
        if(it!=m_all.end()&&it->second!=NULL) return true;
        else return false;
    };

    ///获取数据集中合约ID的集合
    void GetAllInstrumentsID(set<string>& outData)
    {
        outData.clear();
        for(map<string,set<long>>::iterator it=m_allIndex2.begin();it!=m_allIndex2.end();it++)
        {
            if(outData.find(it->first)==outData.end())
                outData.insert(it->first);
        }
    }
    ///判断是否为空
    bool empty(void)
    {
        return m_all.empty();
    }
    ///取所有key
    void GetAllKeys(vector<KeyType>& outData)
    {
        outData.clear();
        for(map<KeyType,DataStruType*>::const_iterator it=m_all.begin();it!=m_all.end();it++)
            outData.push_back(it->first);
    }



    //----------下面是数据集----------
    string                          m_strQryInstrument;     //查询对应的合约代码，为空时表示查询全部
    vector<DataStruType>            m_QryRlt;               //查询的结果
    map<KeyType,int>                m_mapKey2QryRlt;        //查询结果的Key to vector order 的map
    set<string>                     m_InstrumentsInQryRlt;  //查询结果中的合约列表

    map<KeyType,DataStruType*>      m_all;                  //Key对应的全部记录
    map<long,DataStruType*>         m_all2;                 //FTID对应的全部记录
    map<string,set<KeyType>>        m_allIndex;             //合约对应的索引集合。
    map<string,set<long>>           m_allIndex2;            //合约对应的FTID的集合。
    set<long>                       m_allFTID;              //全部FTID集合
    map<long,long>                  m_UpdateSeq2FTID;       //UpdateSeq对应的FTID
    bool                            m_bUpdateSeq2FTID;      //是否要维护UpdateSeq和FTID的对应关系。成交和报单要维护，用于增量落地

private:

    bool m_bNeedKey2QryRlt;                                 //查询结果返回时，是否需要维护m_mapKey2QryRlt. 对于持仓，需要维护，用于合并当日持仓和历史持仓；组合持仓明细也需要。其它的不用
    volatile long& m_baseFTID;                              //FTID基准值，每次使用后递增
    volatile long& m_baseUpdateSeq;                         //UpdateSeq基准值，每次使用后递增
    long m_lastUpdateSeq;                                   //最后一次修改的UpdateSeq值，常用来表示数据有没有更新
};

#ifdef WIN32
#pragma managed(pop)
#endif 
