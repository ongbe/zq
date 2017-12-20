//定制的列表框类。
//用于所有委托单、未成交单、成交记录、持仓、持仓明细、组合持仓等模块
//列表框和数据集一一对应，<KeyType,DataStruType>
//每个数据集中均包含InstrumentID、FTID、UpdateSeq三个字段
//FTID用于标识唯一的key
//UpdateSeq用于表示数据是否有变化


#pragma once

#include "ExtListCtrl.h"
#include "GlobalConfigMgr.h"
#include <string>
#include <set>
#include <map>
#include <vector>

#include "SimpleWriteLog.h"

#if 0
#define LOG_INFO_DataListCtrl(fmt, ...) 
#else
#define LOG_INFO_DataListCtrl(fmt, ...) \
    do{\
        char logbuf[256],logbuf2[256];\
        logbuf[0]=1;  logbuf2[0]=1;\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataListCtrl", LOGLEVEL_DEBUGINFO, "[%d]:"fmt, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


class wxExtListHeaderWindow;


//回调函数，用于对比新旧数据。
//如果某个数据项不同，则将新数据更新到旧数据，并生成Item，返回后进行界面更新。返回值表示数据是否有变化
//pNewData和pOldData都是指向一行数据的指针
//FieldID表示要更新的数据项的ID
//bnewline表示是不是新行，如果是新行，数据已经更新了，直接生成Item
typedef	bool	(*UpdateListItemCB)(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);

template <typename KeyType,typename DataStruType>
class CDataListCtrl : public wxExtListCtrl
{
public:
    CDataListCtrl(  UpdateListItemCB updateCB,
                    wxWindow *parent,
                    wxWindowID winid = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxLCEXT_REPORT,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString &name = wxEmptyString):
    wxExtListCtrl(parent, winid, pos, size, style, validator, name)
    {
        m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"DataListCtrl.log");

        m_UpdateCB=updateCB;
        m_TextCol=0;
	    InitializeCriticalSection(&m_CS);

        m_all.clear();
        m_all2.clear();
        m_allIndex.clear();
        m_allIndex2.clear();
    };

    ~CDataListCtrl()
    {
        Clear();

        DeleteCriticalSection(&m_CS);
        if(m_pWriteLog)
        {
            delete m_pWriteLog;
            m_pWriteLog=NULL;
        }
    };

    void Clear2_Internal(void)
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

    void SetCfg(unsigned long textcol,std::map<int, long>& fieldID2ColID)
    {
        m_TextCol=textcol;
        m_FieldID2ColID=fieldID2ColID;
    };

    void Clear(void)
    {
        DeleteAllItems();

        EnterCriticalSection(&m_CS);

        Clear2_Internal();

        LeaveCriticalSection(&m_CS);
    };

    void UpdateAllItems(std::vector<DataStruType>& vecData)
    {
        bool bNeedResort,tmpbNeedResort;

        wxGetTopLevelParent(this)->SetCursor(*wxHOURGLASS_CURSOR);
        Clear();

        Freeze();

        EnterCriticalSection(&m_CS);
        bNeedResort=false;

        int count=vecData.size();
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecData[i],tmpbNeedResort, false);
            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        if(GetSortCol()!=-1)
        {
            //列表框小于最大排序行数时，才自动排序，以免影响速度
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

        Thaw();
        wxGetTopLevelParent(this)->SetCursor(*wxSTANDARD_CURSOR);
    };


    //setFTID用于删除不再需要的项，setFTID和vecValue数量一致，用于更新内容
    void UpdateInstrumentItems2(std::string strInstrument,
                                std::set<long>& setFTID,
                                std::vector<DataStruType>& vecValue)
    {
        bool bNeedResort,tmpbNeedResort;
        int count=setFTID.size();

        if(count!=vecValue.size())
            return;
        
        EnterCriticalSection(&m_CS);

        bNeedResort=false;

        //逐行更新
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecValue[i],tmpbNeedResort,true);

            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        //检查有没有要删除的项
        static bool bgo1=true;
        if(bgo1)
        {
            typename std::map<long, DataStruType>::iterator it_oldData2;
            std::map<std::string,std::set<long>>::iterator it2;
            long FTID;

            it2=m_allIndex2.find(strInstrument);
            if(it2!=m_allIndex2.end()&&
                !it2->second.empty()&&
                it2->second.size()!=setFTID.size())
            {
                std::set<long>::iterator itold=it2->second.begin();
                std::set<long>::iterator itnew=setFTID.begin();

                //查找要删除的项
                while(itold!=it2->second.end())
                {
                    FTID=(*itold);
                    itold++;

                    while(itnew!=setFTID.end()&&(*itnew)<FTID) itnew++;
                    if(itnew==setFTID.end()||(*itnew)!=FTID)
                    {
                        //itnew里没有对应项目，需要删除
                        DeleteLineByFTID_Internal(FTID);
                        DeleteRecord3_Internal(FTID);

                        if(m_allIndex2.find(strInstrument)==m_allIndex2.end())
                            break;
                    }
                }
            }

        }


        static bool bgo2=true;
        if(GetSortCol()!=-1&&bgo2)
        {
            //列表框小于最大排序行数时，才自动排序，以免影响速度
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

    };

    //不会删除数据
    void UpdateInstrumentItems3(std::string strInstrument,
                                std::vector<DataStruType>& vecValue)
    {
        bool bNeedResort,tmpbNeedResort;
        int count=vecValue.size();

        EnterCriticalSection(&m_CS);

        bNeedResort=false;

        //逐行更新
        for(int i=0;i<count;i++)
        {
            UpdateOneItem_Internal(vecValue[i],tmpbNeedResort,true);

            if(!bNeedResort&&tmpbNeedResort)
                bNeedResort=true;
        }

        static bool bgo2=true;
        if(GetSortCol()!=-1&&bgo2)
        {
            //列表框小于最大排序行数时，才自动排序，以免影响速度
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);

    };

    void UpdateOneItem(DataStruType& value)
    {
        bool bNeedResort;

        EnterCriticalSection(&m_CS);

        UpdateOneItem_Internal(value, bNeedResort,true);

        if(GetSortCol()!=-1)
        {
            //列表框小于最大排序行数时，才自动排序，以免影响速度
            if(GetItemCount()>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl())
                DisableSortCol();
            else if(bNeedResort) 
                ReSortItems();
        }

        LeaveCriticalSection(&m_CS);
    }

    // 删除一行记录
    void DeleteOneItem(const KeyType& key)
    {
        std::map<KeyType, DataStruType*>::iterator it;

        EnterCriticalSection(&m_CS);

        it=m_all.find(key);
        if(it!=m_all.end()&&it->second!=NULL) 
        {
            long FTID=it->second->FTID;

            DeleteLineByFTID_Internal(FTID);

            DeleteRecord3_Internal(FTID);
        }

        LeaveCriticalSection(&m_CS);
    }



    void UpdateOneItem_Internal(const DataStruType& value,bool& bNeedResort, const bool bRefreshUI)
    {
        bool bnewline=false;
        int LineNum,ColNum;
        long FTID=value.FTID;

        bNeedResort=false;

        std::map<long, DataStruType*>::iterator it_oldData2;

        it_oldData2=m_all2.find(FTID);
        if(it_oldData2!=m_all2.end()&&
            it_oldData2->second!=NULL&&
            it_oldData2->second->UpdateSeq==value.UpdateSeq)
            return;                                             //数据没有改变

        std::string strInstrument=std::string(value.InstrumentID);
        int rowid;
        LineNum=GetItemCount();
        ColNum=GetColumnCount();

        if(ColNum<=0) 
            return;

        if(it_oldData2==m_all2.end())
        {
            //插入新行
            bnewline=true;
            UpdateRecord_Internal(value);

            //因为总是插入到最后一行，不会影响原有记录。在下面的SetItem更新UI即可，这里不用更新
            rowid=InsertItem2(LineNum,wxString(""),FTID,true);
            LineNum++;

            it_oldData2=m_all2.find(FTID);
        }
        else
        {
            rowid=FindItem2(FTID);
        }

        if(it_oldData2==m_all2.end())
            return;                                             //这是不可能的
        if(rowid>=LineNum)
            return;                                             //正常情况下也是不可能的

        //获取排序的列序号
        int sortcol=GetSortCol();

        std::map<int, long>::iterator it_col;
        bool brlt;
        bool bUpdate=false;
        std::vector<wxExtListItem*> UpdateItems;

        //逐列更新
        for(it_col=m_FieldID2ColID.begin();it_col!=m_FieldID2ColID.end();it_col++)
        {
            if(it_col->second<0||
                it_col->second>=ColNum) continue;

            wxExtListItem *pItem=new wxExtListItem;
            brlt=m_UpdateCB(&value,it_oldData2->second,m_TextCol,it_col->first,it_col->second,rowid,bnewline,*pItem);

            if(brlt)
            {
                if(!bUpdate) bUpdate=true;

                if(it_col->second==0)
                {
                    //注意：修改第一列单元格时，有可能会改掉行相关的UserData。所以这里进行保护，清除ItemData的Mask
                    pItem->HoldItemData();
                }

                UpdateItems.push_back(pItem);

                if(it_col->second==sortcol&&sortcol>=0&&sortcol<ColNum&&!bNeedResort)
                    bNeedResort=true;
            }
            else 
            {
                delete pItem;
            }
        }

        if(UpdateItems.size()>0)
        {
            SetItems(UpdateItems,bRefreshUI);

            LOG_INFO_DataListCtrl("UpdateOneItem_Internal: SetItems, UpdateItems.size()=%d FTID=%u bRefreshUI=%d LineNum=%d",UpdateItems.size(),FTID,bRefreshUI,LineNum);

            int i,isize=UpdateItems.size();
            for(i=0;i<isize;i++)
            {
                delete UpdateItems[i];
            }
            UpdateItems.clear();
        }

        if(!bnewline)
        {
            if(bUpdate) *(it_oldData2->second)=value;
            else it_oldData2->second->UpdateSeq=value.UpdateSeq;
        }
    };
    

    //删除FTID指定的行
    void DeleteLineByFTID_Internal(long FTID)
    {
        int row=FindItem2(FTID);
        if(row>=0)
            DeleteItem(row);
    };

    //取指定行的数据
    bool GetValueByRow(int rowid, DataStruType& value)
    {
        bool brlt=false;
        EnterCriticalSection(&m_CS);
        long FTID=GetItemData(rowid);
        std::map<long, DataStruType*>::iterator it2;

        if((it2=m_all2.find(FTID))!=m_all2.end()&&it2->second!=NULL)
        {
            value=*(it2->second);
            brlt=true;
        }
        LeaveCriticalSection(&m_CS);
        return brlt;
    }

private:
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
    void UpdateRecord_Internal(const DataStruType& DataInfo)
    {
        std::map<long,DataStruType*>::iterator it_all2;
        KeyType tmpkey(DataInfo);
        std::string strInstrument(tmpkey.InstrumentID);
        long FTID;

        FTID=DataInfo.FTID;
        it_all2=m_all2.find(FTID);
        if(it_all2!=m_all2.end()&&it_all2->second!=NULL)
        {
            //更新原有数据
            *(it_all2->second)=DataInfo;
        }
        else
        {
            //插入新数据
        
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
        }
    }


private:

    CRITICAL_SECTION m_CS;

    std::map<std::string, std::set<KeyType>> m_allIndex;        //InstrumentID到set of key的映射
    std::map<std::string, std::set<long>> m_allIndex2;          //InstrumentID到set of key的映射
	std::map<KeyType, DataStruType*> m_all;		                //Key到Value的对应Map
	std::map<long, DataStruType*> m_all2;		                //Key到Value的对应Map

    UpdateListItemCB m_UpdateCB;
    unsigned long m_TextCol;                                    //通用的字体颜色
    std::map<int, long> m_FieldID2ColID;                        //列到字段ID的映射

    zqWriteLog*           m_pWriteLog;

};