#include <map>
#include <vector>
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonMacro.h"
/*
static char *pkgWrap(std::map<int,FinancialProduct>& mapFinancialProduct,
                     std::vector<TraderProRelation>& vecTraderProRelation, 
                     std::vector<FinancialProduct>& vecProAdd,
                     std::map<int,FinancialProduct>& mapProDel,
                     std::map<int,FinancialProduct>& mapProMod,
                     std::vector<TraderProRelation>& vecRelaAdd,
                     std::vector<TraderProRelation>& vecRelaDel)
{
    vecProAdd.clear();
    mapProDel.clear();
    mapProMod.clear();
    vecRelaAdd.clear();
    vecRelaDel.clear();

    std::map<int,FinancialProduct>::iterator it = mapFinancialProduct.begin();
    for (;it != mapFinancialProduct.end(); it++)
    {
        FinancialProduct oFP = it->second;
        switch(oFP.nDBOprType)
        {
        case Cmd_RM_AddFinicialProduct_Req:
            {
                vecProAdd.push_back(oFP);
                break;
            }
        case Cmd_RM_DeleteFinicialProduct_Req:
            {
                mapProDel[oFP.nID] = oFP;
                break;
            }
        case Cmd_RM_ModifyFinicialProduct_Req:
            {
                mapProMod[oFP.nID] = oFP;
                break;
            }
        }
    }

    std::vector<TraderProRelation>::iterator itRela = vecTraderProRelation.begin();
    for (;itRela != vecTraderProRelation.end(); itRela++)
    {
        TraderProRelation oRela = *itRela;
        switch(oRela.nDBOprType)
        {
        case Cmd_RM_AddProductTraderRelation_Req:
            {
                vecRelaAdd.push_back(oRela);
                break;
            }
        case Cmd_RM_DeleteProductTraderRelation_Req:
            {
                vecRelaDel.push_back(oRela);
                break;
            }
        }
    }


    int nProductLen = vecProAdd.size()*sizeof(FinancialProduct) + 
        mapProDel.size()*sizeof(FinancialProduct) + 
        mapProMod.size()*sizeof(FinancialProduct);

    int nRelationLen = vecRelaAdd.size()*sizeof(TraderProRelation) + 
        vecRelaDel.size()*sizeof(TraderProRelation);

    char *pData = NULL;    
    if(nProductLen == 0 && nRelationLen == 0)
        return pData;
    else
    {
        pData = new char[8+nProductLen+nRelationLen];
        memset(pData,0,8+nProductLen+nRelationLen);
    } 

    char *pReturn = pData;
    memcpy(pData,&nProductLen,sizeof(int));
    pData += sizeof(int);
    memcpy(pData,&nRelationLen,sizeof(int));
    pData += sizeof(int);

    std::vector<FinancialProduct>::iterator itProAdd = vecProAdd.begin();
    for (; itProAdd!=vecProAdd.end(); itProAdd++)
    {
        FinancialProduct oFP = *itProAdd;
        memcpy(pData,&oFP,sizeof(FinancialProduct));
        pData += sizeof(FinancialProduct);
    }

    std::map<int,FinancialProduct>::iterator itProDel = mapProDel.begin();
    for (; itProDel!=mapProDel.end(); itProDel++)
    {
        FinancialProduct oFP = itProDel->second;
        memcpy(pData,&oFP,sizeof(FinancialProduct));
        pData += sizeof(FinancialProduct);
    }

    std::map<int,FinancialProduct>::iterator itProMod = mapProMod.begin();
    for (; itProMod!=mapProMod.end(); itProMod++)
    {
        FinancialProduct oFP = itProMod->second;
        memcpy(pData,&oFP,sizeof(FinancialProduct));
        pData += sizeof(FinancialProduct);
    }

    std::vector<TraderProRelation>::iterator itRelaAdd = vecRelaAdd.begin();
    for (; itRelaAdd!=vecRelaAdd.end(); itRelaAdd++)
    {
        TraderProRelation oRela = *itRelaAdd;
        memcpy(pData,&oRela,sizeof(TraderProRelation));
        pData += sizeof(TraderProRelation);
    }

    std::vector<TraderProRelation>::iterator itRelaDel = vecRelaDel.begin();
    for (; itRelaDel!=vecRelaDel.end(); itRelaDel++)
    {
        TraderProRelation oRela = *itRelaDel;
        memcpy(pData,&oRela,sizeof(TraderProRelation));
        pData += sizeof(TraderProRelation);
    }

    return pReturn;
}

static bool UnPkgWrap(int PkgHeadLen,
                      const char* pPkgData,
                      std::vector<FinancialProduct>& vecProAdd,
                      std::map<int,FinancialProduct>& mapProDel,
                      std::map<int,FinancialProduct>& mapProMod,
                      std::vector<TraderProRelation>& vecRelaAdd,
                      std::vector<TraderProRelation>& vecRelaDel)
{
    char *pData = (char*)pPkgData;

    int nProductStructLen = *(int*)pData;
    pData = (char*)pData+4;
    int nRelationLen = *(int*)pData;
    pData = (char*)pData+4;

    if(PkgHeadLen != (2*sizeof(int) + nProductStructLen + nRelationLen))
        return false;

    for ( int i = 0; i < (int)(nProductStructLen/sizeof(FinancialProduct)); i++ )
    {
        FinancialProduct* pFP = (FinancialProduct*)((char*)(pData + i*sizeof(FinancialProduct)));  
        switch(pFP->nDBOprType)
        {
        case Cmd_RM_AddFinicialProduct_Req:
            {
                vecProAdd.push_back(*pFP);
                break;
            }
        case Cmd_RM_DeleteFinicialProduct_Req:
            {
                mapProDel[pFP->nID] = *pFP;
                break;
            }
        case Cmd_RM_ModifyFinicialProduct_Req:
            {            
                mapProMod[pFP->nID] = *pFP;
                break;
            }
        }
    } 

    pData = (char*)pData+nProductStructLen;

    for (int j = 0; j<(int)(nRelationLen/sizeof(TraderProRelation)); j++)
    {
        TraderProRelation* pRelation = (TraderProRelation*)((char*)(pData + j*sizeof(TraderProRelation)));
        switch(pRelation->nDBOprType)
        {
        case Cmd_RM_AddProductTraderRelation_Req:
            {
                vecRelaAdd.push_back(*pRelation);
                break;
            }
        case Cmd_RM_DeleteProductTraderRelation_Req:
            {
                vecRelaDel.push_back(*pRelation);
                break;
            }
        }
    }

    return true;
}*/