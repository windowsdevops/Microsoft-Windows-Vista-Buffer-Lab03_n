'
' S3ResumeLogo.vbs
'
' PwrTest example script
'
' usage: S3ResumeLogo.vbs <pstrace-file.xml>
' date: 14 September 2005
'
' evaluate the system S3 resume time and generate 
' a success or failure entry in the DTM log.
'
Option Explicit
'----------------------------
' Declarations
'----------------------------
'Const PERF_PWR_TEST_COMMAND = ".\PwrTest.exe /sleep /h:n"
Const PERF_PWR_TEST_COMMAND = ".\PwrTest.exe /sleep /c:4 /d:90 /p:90 /h:n /s:3"
Const PERF_PWRTESTLOG_XML_FILE = "PWRTESTLOG.XML"
'Const MIN_RESUME_TIME_MS	= 2000
Const MAX_RESUME_TIME = 2000
Const MAX_RESUME_TIME_RELAXED = 7000

Const GUIDOfS3Resume = "C98100E4-BF10-431e-B803-BD1292276285"


Const PERF_XML_NODE_LIST_SEARCH_PREFIX = "//"
Const PERF_XML_SLEEP_TRANSITION_NODE_NAME = "SleepTransition"
Const PERF_XML_SYSTEM_POWER_CAPABILITIES_NODE_NAME = "SYSTEM_POWER_CAPABILITIES"

Const PERF_XML_EFFECTIVE_STATE_NAME = "EffectiveState"
Const PERF_XML_BIOSINIT_NAME = "BIOSInit"
Const PERF_XML_RESUME_NAME = "Resume"
Const PERF_XML_DRIVERINIT_NAME = "DriverInit"

Const PERF_XML_S3_SUPPORTED_NAME = "SystemS3StateSupported"
Const PERF_XML_RTC_WAKE_SUPPORTED_NAME = "RtcWakeSupported"

Const PERF_ERROR_MSG_LOG_FILE_NOT_VALID = "The PwrTestLog File Could Not Be Found."
Const PERF_MSG_LOG_OPENED_SUCCESSFULLY = "The PwrTestLog File Was Opened Successfuly:"
Const EXIT_GENERIC_FATAL = 255
Const S3LogTestName="S3ResumeLogoTest"


'WTTLog globals
const WTT_TRACE_LVL_MSG                   =268435456
const WTT_TRACE_LVL_USER                  =536870912
const WTT_TRACE_LVL_ERR                   =16777216
const WTT_TRACE_LVL_ASSERT                =33554432
const WTT_TRACE_LVL_INVALID_PARAM         =67108864
const WTT_TRACE_LVL_BUG                   =134217728
const WTT_TRACE_LVL_BREAK                 =1048572
const WTT_TRACE_LVL_WARN                  =2097152
const WTT_TRACE_LVL_FUNCTION              =65536
const WTT_TRACE_LVL_PARAM                 =131072
const WTT_TRACE_LVL_START_TEST            =256
const WTT_TRACE_LVL_END_TEST              =512
const WTT_TRACE_LVL_TCMINFO               =1024
const WTT_TRACE_LVL_MACHINEINFO           =2048
const WTT_TRACE_LVL_ROLLUP                =16

const WTT_TC_RESULT_PASS      = 1
const WTT_TC_RESULT_FAIL      = 2
const WTT_TC_RESULT_BLOCKED   = 3
const WTT_TC_RESULT_WARN      = 4
const WTT_TC_RESULT_SKIPPED   = 5

Const WTTLOG_TESTCASE_TYPE_NOPARENT		= 1
Const WTTLOG_TESTCASE_TYPE_ISCHILD		= 2
Const WTTLOG_TESTCASE_TYPE_ISSIBLING		= 3
Const WTTLOG_TESTCASE_TYPE_SPECIFYPARENT=4

'---------------------------------
' Globals
'---------------------------------
Dim g_WTTLog
Dim g_WTTHandle
Dim g_MaxResumeTime
Dim g_Option
Dim g_bRelaxResumeTimeRequirement 
Dim g_Effective 
Dim g_bDebug
Dim g_bLogPass
Dim g_S3SleepPerfTraceFileDOM
'---------------------------------
' Main
'---------------------------------
PerfMain


Sub PerfMain

	g_Option = 0
	g_bDebug=True
	
	PerfHandleCommandLineArguments

	Select Case(g_Option)
	Case 1:
		PerfRunProgram
	Case 2:
		PerfParseXmlFile
   case 3:
      PerfRunProgram
      PerfParseXmlFile
	Case Else
		PerfDisplayUsage
	End Select
	
End Sub
'End Main


Sub PerfRunProgram
	Dim RetRun
	Dim wshShell
	Set WshShell = WScript.CreateObject("WScript.Shell")

	RetRun = WshShell.Run(PERF_PWR_TEST_COMMAND, 1, true)
	'RetRun = WshShell.Sleep(100)
End Sub

Sub PerfParseXmlFile
	Set g_WTTLog = CreateObject("WTTLogger.Logger")

	g_WTTHandle = g_WTTLog.CreateLogDevice("$LocalPub($LogFile:writemode=overwrite ,encoding=UNICODE;$Console)")
	g_WTTLog.StartTestEx GUIDOfS3Resume, S3LogTestName, WTTLOG_TESTCASE_TYPE_NOPARENT, 0, 0, g_WTTHandle

	' Try to get the resume time and do logo test
	Dim WTTResult
	WTTResult = WTT_TC_RESULT_PASS

	g_bLogPass = PerfIsLogFileExist
	If(1=g_bLogPass)	Then
		'
		' Get XML DOM to open / parse the file
		'
		PerfOpenAdvancedPerformanceTraceFileWithDOM g_S3SleepPerfTraceFileDOM, PERF_PWRTESTLOG_XML_FILE 
		
		g_bLogPass = PerfGetPowerCapabilities()
		If(1=g_bLogPass)	Then
			g_bLogPass = PerfGetResumeTime()
		End If
		Set g_S3SleepPerfTraceFileDOM = Nothing
	End If

	If( 0=g_bLogPass )	Then
		WTTResult = WTT_TC_RESULT_FAIL
	End If

	g_WTTLog.EndTestEx WTTLOG_TESTCASE_TYPE_NOPARENT, GUIDOfS3Resume, S3LogTestName, 0, 0, g_WTTHandle
	g_WTTLog.Trace WTT_TRACE_LVL_ROLLUP, g_WTTHandle, 0, 0, 0, 0, 0
	g_WTTLog.CloseLogDevice 0, g_WTTHandle

	Set g_WTTLog=Nothing
End Sub

'
' PerfHandleCommandLineArguments
'
' - Makes sure that one command line argument exists,
' - that the file can be opened and displays usage
' - otherwise
'
Sub PerfHandleCommandLineArguments

   if( WScript.Arguments.Count = 0)  Then
   
      g_Option = 3
      
      g_MaxResumeTime = MAX_RESUME_TIME
      
   Elseif (WScript.Arguments.Count > 2) Then 

      g_Option = 0
      
   ElseIf (WScript.Arguments.Item (0) = "/?") OR (WScript.Arguments.Item(0) = "-?") Then
      
      g_Option = 0
      
   Else
   
      g_Option = CInt(WScript.Arguments.Item (0))
      
   End If
         
   If(g_Option = 2) Then
   
      If (WScript.Arguments.Count = 1) Then

         g_MaxResumeTime = MAX_RESUME_TIME
         
      ElseIf(WScript.Arguments.Item (1) = "FALSE") Then
      
         g_MaxResumeTime = MAX_RESUME_TIME
         
      ElseIf(WScript.Arguments.Item (1) = "TRUE") Then
         
         g_MaxResumeTime = MAX_RESUME_TIME_RELAXED
         
      Else
      
         g_Option = 0
         
      End If
      
   End If
	
End Sub

'
' PerfDisplayUsage
'
' - Displays the usage information for the script
'
Sub PerfDisplayUsage 

	WScript.Echo(" ")
	WScript.Echo("Usage: S3ResumeLogo.vbs <nOption>")
	WScript.Echo("  nOption:")
	WScript.Echo("        1--Run pwrtest.exe")
	WScript.Echo("        2--Parse powerspylog.xml ")
	WScript.Echo(" ")
	WScript.Echo("  Examples:")
	WScript.Echo("    S3ResumeLogo.vbs 1")
	WScript.Echo("    S3ResumeLogo.vbs 2")
	WScript.Echo(" ")
	

End Sub


'
' PerfLogToPerfFileAndDisplay 
'
' - Writes the text string as a line to the S3ResumeLogo log file
'   and the standard output
'
Sub PerfLogToPerfFileAndDisplay  (Text)

	'WScript.Echo (Text)
 	g_WTTLog.Trace WTT_TRACE_LVL_MSG,      g_WTTHandle, Text

End Sub

'
' PerfDebugOutput 
'
' - Writes the text string as a line to the S3ResumeLogo log file
'   and the standard output
'
Sub PerfDebugOutput  (Text)
	If( g_bDebug ) 	Then
		'WScript.Echo (Text)
	 	g_WTTLog.Trace WTT_TRACE_LVL_MSG,      g_WTTHandle, Text
	 End If
End Sub


'
' PerfIsLogFileExist
'
' - Makes sure that one command line argument exists,
' - that the file can be opened and displays usage
' - otherwise
'
Function PerfIsLogFileExist
	'
	' Test that the argument is a legit file
	' by opening it
	'
	Err.Clear
	
	Dim tempFile
	Dim FileSystemObject

	Set FileSystemObject = WScript.CreateObject("Scripting.FileSystemObject")
	On Error Resume Next
	Set tempFile = FileSystemObject.OpenTextFile(PERF_PWRTESTLOG_XML_FILE)
	tempFile.Close()

	PerfIsLogFileExist = 1
	If (Err.Number) Then

		PerfLogToPerfFileAndDisplay (PERF_ERROR_MSG_LOG_FILE_NOT_VALID)
		Err.Clear
		PerfIsLogFileExist = 0
		Exit Function

	End If

	PerfLogToPerfFileAndDisplay (PERF_MSG_LOG_OPENED_SUCCESSFULLY & " " & PERF_PWRTESTLOG_XML_FILE )

End Function


'
' PerfGetPowerCapabilities
'
' If( 0=SystemS3StateSupported ) Then Fail
' If(( 0=SystemS3StateSupported ) And (( 3<RtcWakeSupported ) Then Fail and log something
'
Function PerfGetPowerCapabilities()

	PerfLogToPerfFileAndDisplay ( "PerfGetPowerCapabilities: Start to parse the SYSTEM_POWER_CAPABILITIES")

	'
	' See If we can spew events
	'
	Dim EventNodeList
	Set EventNodeList = g_S3SleepPerfTraceFileDOM.SelectNodes(PERF_XML_NODE_LIST_SEARCH_PREFIX & PERF_XML_SYSTEM_POWER_CAPABILITIES_NODE_NAME)

	Dim S3Support
	Dim RtcWakeSupport

	Dim xEventNode
	Dim EventNode

	Dim RetValue

	If( EventNodeList.length>0 )	Then
		RetValue = 1
		xEventNode = 0


		Set EventNode=EventNodeList.Item(xEventNode)

		PerfGetTextForId EventNode, PERF_XML_S3_SUPPORTED_NAME, S3Support
		PerfGetTextForId EventNode, PERF_XML_RTC_WAKE_SUPPORTED_NAME, RtcWakeSupport


		PerfLogToPerfFileAndDisplay ("  PerfGetPowerCapabilities: S3Support: " & S3Support & "; RtcWakeSupport: " & RtcWakeSupport)
		
		If(  CInt(S3Support)=0 )Then
			RetValue = 0
			g_WTTLog.Trace WTT_TRACE_LVL_ASSERT, g_WTTHandle, "S3Resume.vbs", 123, "The machine doesn't support S3 state"
		End If

		If(  (CInt(RtcWakeSupport)<3) And CInt(S3Support)=1  )Then
			RetValue = 0
			g_WTTLog.Trace WTT_TRACE_LVL_ASSERT, g_WTTHandle, "S3Resume.vbs", 123, "The machine doesn't support RTC wake from S3 state"
		End If
	End If
	
	PerfGetPowerCapabilities = RetValue
	If(  RetValue=1 )Then
		PerfLogToPerfFileAndDisplay ("PerfGetPowerCapabilities: Succeed")
	Else
		PerfLogToPerfFileAndDisplay ("PerfGetPowerCapabilities: Failed")
	End If
End Function


'
' PerfGetResumeTime
'
' Igonore the first power transition, and caculate the following transistions with
'	 TotalResumeTime = ResumeTime + BiosInitTime
'
'	If Any TotalResumeTime>MIN_RESUME_TIME_MS ms Then Fail (0)
'	Else Pass(1)
'
Function PerfGetResumeTime()

	PerfLogToPerfFileAndDisplay ( "PerfGetResumeTime: Start to parse the sleep scenarios")
	'
	' See If we can spew events
	'
	Dim EventNodeList
	Set EventNodeList = g_S3SleepPerfTraceFileDOM.SelectNodes(PERF_XML_NODE_LIST_SEARCH_PREFIX & PERF_XML_SLEEP_TRANSITION_NODE_NAME)
	'WScript.Echo (EventNodeList.length)

	Dim ResumeTime
	Dim BiosInitTime
   Dim DriverInitTime 
	Dim EffectiveState
	
	Dim TotalResumeTime
	Dim xEventNode
	Dim EventNode

	Dim RetValue

	RetValue = 1
	xEventNode = EventNodeList.length-1
   
   If (g_MaxResumeTime = MAX_RESUME_TIME_RELAXED) Then
         PerfLogToPerfFileAndDisplay ("  Note: User idenitified this system as having fully-buffered memory module. Resume Time requirement will be relaxed." )
   End If

	While( xEventNode>=1 )

		Set EventNode=EventNodeList.Item(xEventNode)

		PerfGetTextForId EventNode, PERF_XML_BIOSINIT_NAME, BiosInitTime
      PerfGetTextForId EventNode, PERF_XML_DRIVERINIT_NAME, DriverInitTime 
		PerfGetTextForId EventNode, PERF_XML_RESUME_NAME, ResumeTime
		PerfGetTextForId EventNode, PERF_XML_EFFECTIVE_STATE_NAME, EffectiveState

		TotalResumeTime = CInt(BiosInitTime) + CInt(DriverInitTime)

		PerfLogToPerfFileAndDisplay ("  EffectiveState = " & EffectiveState )
      
      PerfLogToPerfFileAndDisplay ("  Bios Initialization Phase Time =  " & BiosInitTime )
      
      PerfLogToPerfFileAndDisplay ("  Driver Initialization Phase Time =  " & DriverInitTime )
      
      PerfLogToPerfFileAndDisplay ("  Total Resume Time = " & TotalResumeTime )

		If  (TotalResumeTime>=g_MaxResumeTime)	Then
			RetValue = 0
			g_WTTLog.Trace WTT_TRACE_LVL_ASSERT, g_WTTHandle, "S3Resume.vbs", 349, "The total resume time of this machine is longer than " & g_MaxResumeTime & "ms"
		
      ElseIf  (Cint(DriverInitTime) >= MAX_RESUME_TIME)	Then
			RetValue = 0
			g_WTTLog.Trace WTT_TRACE_LVL_ASSERT, g_WTTHandle, "S3Resume.vbs", 349, "The DriverInit Phase of the Resume on this machine took longer than " & MAX_RESUME_TIME & "ms"
		End If


		If( CInt(EffectiveState)<3 )	Then
			RetValue = 0
			g_WTTLog.Trace WTT_TRACE_LVL_ASSERT, g_WTTHandle, "S3Resume.vbs", 354, "The machine doesn't really sleep in S3 State"
		End If

		xEventNode = xEventNode - 1
	WEnd

	PerfGetResumeTime = RetValue
	If(  RetValue=1 )Then
		PerfLogToPerfFileAndDisplay ("PerfGetResumeTime: Succeed")
	Else
		PerfLogToPerfFileAndDisplay ("PerfGetResumeTime: Failed")
	End If

End Function



'
' PerfGetEventIDFromSystemNode
'
' - Returns a single event node with a matching EventID
'
Sub PerfGetTextForId (EventNode, IdName, ByRef EventID)

	'
	' Get the EventID node data
	'
	Dim EventIDNode
	Set EventIDNode = EventNode.SelectSingleNode(IdName)
	EventID = EventIDNode.Text
	
	'WScript.Echo ( IdName & ": " & EventID)

End Sub


'
' PerfOpenAdvancedPerformanceTraceFileWithDOM
'
' - Opens the advanced performance log file using
'   the DOM XML parser
'
Sub PerfOpenAdvancedPerformanceTraceFileWithDOM (ByRef PerfLogFileXML, PerfLogFileName)
	Dim PerfSpyLogFileXML
	
	Set PerfLogFileXML = CreateObject("MSXML.DOMDocument")
	PerfLogFileXML.validateOnParse = True
	
	PerfLogFileXML.Load(PerfLogFileName)
	
	If (PerfLogFileXML.parseError.errorCode <> 0) Then
		Dim PerfLogFileXMLError

   		Set PerfLogFileXMLError = PerfLogFileXML.parseError
		PerfLogToPerfFileAndDisplay("*** PowerSpy Log File Error Detected ***")
		PerfLogToPerfFileAndDisplay(" ")
		PerfLogToPerfFileAndDisplay("LineNumber: " & PerfLogFileXMLError.line)
		PerfLogToPerfFileAndDisplay("LinePos:    " & PerfLogFileXMLError.linepos)
		PerfLogToPerfFileAndDisplay("Text:       " & PerfLogFileXMLError.srcText)
		PerfLogToPerfFileAndDisplay("Reason:     " & PerfLogFileXMLError.reason)
		PerfLogToPerfFileAndDisplay("****************************************")
		
		Set PerfSpyLogFileXML = nothing

	End If

End Sub
