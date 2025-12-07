![Alt text](Untitled.png "Optional title")

SET MSSdk=E:\WinDDK\6000\ or the Root Directory in SetEnv.bat

Read the Useage and execute the batch file.

<p>:: --------------------------------------------------------------------------------------------
:: File    : SetEnv.cmd
::
:: Abstract: This batch file sets the appropriate environment
::           variables for the Windows SDK build environment with
::           respect to OS and platform type.
::
:: "Usage Setenv [/Debug | /Release][/x86 | /x64 | /ia64][/vista | /xp | /2003 ][-h] "
::
::                /Debug   - Create a Debug configuration build environment
::                /Release - Create a Release configuration build environment
::                /x86     - Create 32-bit x86 applications
::                /x64     - Create 64-bit x64 applications
::                /ia64    - Create 64-bit ia64 applications
::                /vista   - Windows Vista applications
::                /xp      - Create Windows XP SP2 applications
::                /2003    - Create Windows Server 2003 applications
::
:: --------------------------------------------------------------------------------------------</p>

To Build the Buffer Lab or Samples use NMAKE from Microsoft or AT&T.