#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

public ref class BrokerData
{
public:
    BrokerData(int nBrokerID, int nType, String^ strBrokerCode, String^ strCompany, String^ strAccount, String^ strPSW)
    {
        m_nBrokerID = nBrokerID;
        m_nBrokerType = nType;
        m_strCode = strBrokerCode;
        m_strCompany = strCompany;
        m_strAccount = strAccount;
        m_strPSW = strPSW;
    }

    virtual String^ ToString() override
    {
        String^ str = m_strCompany + "(";
        str = str + m_strCode;
        str = str + ")";
        return str;
    }

    int GetBrokerID()
    {
        return m_nBrokerID;
    }

    int GetBrokerType()
    {
        return m_nBrokerType;
    }

    String^ GetBrokerAccount()
    {
        return m_strAccount;
    }

    String^ GetBrokerPSW()
    {
        return m_strPSW;
    }
    String^ GetBrokerCode()
    {
        return m_strCode;
    }

    String^ GetBrokerCompany()
    {
        return m_strCompany;
    }

private:
    int m_nBrokerID; 
    int m_nBrokerType;
    String^ m_strAccount;
    String^ m_strPSW;
public:
    String^ m_strCode;
    String^ m_strCompany;

};
}