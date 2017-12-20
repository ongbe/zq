#pragma once


#include "DataManageEx.h"


using namespace System;
using namespace Extensibility;
using namespace EnvDTE;
using namespace EnvDTE80;
using namespace Microsoft::VisualStudio::CommandBars;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Globalization;

namespace CltStrategy
{
	/// <summary>The object for implementing an Add-in.</summary>
	/// <seealso class='IDTExtensibility2' />
	public ref class Connect : public IDTExtensibility2, public IDTCommandTarget
	{

	public:
		/// <summary>Implements the constructor for the Add-in object. Place your initialization code within this method.</summary>
		Connect()
		{
			m_pDataService = CDataManageEx::NewInstance();
		}

		~Connect()
		{
			m_pDataService->Release();
			m_pDataService = NULL;
			CDataManageEx::DestroyInstance();
		}

		/// <summary>Implements the OnAddInsUpdate method of the IDTExtensibility2 interface. Receives notification when the collection of Add-ins has changed.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />	
		virtual void OnAddInsUpdate(System::Array ^%custom);

		/// <summary>Implements the OnBeginShutdown method of the IDTExtensibility2 interface. Receives notification that the host application is being unloaded.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnBeginShutdown(System::Array ^%custom);

		/// <summary>Implements the OnConnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being loaded.</summary>
		/// <param term='application'>Root object of the host application.</param>
		/// <param term='connectMode'>Describes how the Add-in is being loaded.</param>
		/// <param term='addInInst'>Object representing this Add-in.</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnConnection(System::Object ^Application, ext_ConnectMode ConnectMode, System::Object ^AddInInst, System::Array ^%custom);

		/// <summary>Implements the OnStartupComplete method of the IDTExtensibility2 interface. Receives notification that the host application has completed loading.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnStartupComplete(System::Array ^%custom);

		/// <summary>Implements the OnDisconnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being unloaded.</summary>
		/// <param term='disconnectMode'>Describes how the Add-in is being unloaded.</param>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnDisconnection(ext_DisconnectMode removeMode, System::Array ^%custom);

		/// <summary>Implements the Exec method of the IDTCommandTarget interface. This is called when the command is invoked.</summary>
		/// <param term='commandName'>The name of the command to execute.</param>
		/// <param term='executeOption'>Describes how the command should be run.</param>
		/// <param term='varIn'>Parameters passed from the caller to the command handler.</param>
		/// <param term='varOut'>Parameters passed from the command handler to the caller.</param>
		/// <param term='handled'>Informs the caller if the command was handled or not.</param>
		/// <seealso class='Exec' />
		virtual void Exec(String ^CmdName, vsCommandExecOption ExecuteOption, Object ^%VariantIn, Object ^%VariantOut, bool %handled);

		/// <summary>Implements the QueryStatus method of the IDTCommandTarget interface. This is called when the command's availability is updated</summary>
		/// <param term='commandName'>The name of the command to determine state for.</param>
		/// <param term='neededText'>Text that is needed for the command.</param>
		/// <param term='status'>The state of the command in the user interface.</param>
		/// <param term='commandText'>Text requested by the neededText parameter.</param>
		/// <seealso class='Exec' />
		virtual void QueryStatus(String ^CmdName, vsCommandStatusTextWanted NeededText, vsCommandStatus %StatusOption, Object ^%CommandText);
		void  RemoveComand(String ^CmdName);

	protected:
		Project^ GetProjectByName(EnvDTE90::Solution3^ soln, String^ projName); 
		bool ImportStrategiesAndIndicators(const std::string& strImportFile);
		//bool RunStategyTest(const std::string& strStrategyID, const std::string& strPath);
		bool RunStategyTest(int nRunMode, const std::string& strPath, const std::string& strStrategyID);

	private:
		DTE2 ^_applicationObject;
		AddIn ^_addInInstance;
		CDataManageEx*	m_pDataService;
	};
}
