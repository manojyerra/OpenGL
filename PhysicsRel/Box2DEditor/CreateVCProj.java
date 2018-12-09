import java.io.*;
import java.util.*;

public class CreateVCProj
{
	ScriptReader sr = new ScriptReader("ProjFile_Win32.txt");
	BufferedReader br = new BufferedReader(new FileReader("CreateVCProj.java"));
	BufferedWriter bw;
	Vector additionalDirs = new Vector();
	Vector excludeFoldersVec = null;
	Vector excludeFilesVec = null;
	Vector replacePathsKeys = null;
	Vector replacePathsValues = null;
	
	int numTabs = 0;
	String _projectName;
	
	public static void main(String asdf[])throws Exception
	{
		new CreateVCProj();
	}

	public CreateVCProj()throws Exception
	{
		File vcProjFile = new File("VCProj");
		
		if(!vcProjFile.exists())
			vcProjFile.mkdir();

		excludeFoldersVec = sr.GetTagData("ExcludeFolders");
		excludeFilesVec = sr.GetTagData("ExcludeFiles");
		
		
		if(excludeFilesVec != null)
		{
			for(int i=0;i<excludeFilesVec.size();i++)
			{
				String filePath = (String)excludeFilesVec.get(i);
				File f = new File(filePath);
				f.delete();
				
			}
			excludeFilesVec.removeAllElements();
		}
		
		CreateVCProj();
		CreateSlnFile();
				
		Runtime.getRuntime().exec(new String[]{"cmd.exe", "/c", "start",  "VCProj/"+_projectName+".sln"});
		
		File projClassFile = new File("CreateVCProj.class");
		projClassFile.deleteOnExit();
		
		File scriptClassFile = new File("ScriptReader.class");
		scriptClassFile.deleteOnExit();
	}

	public void CreateVCProj() throws Exception
	{
		_projectName = sr.GetValue("VisualStudioProject", "Name");
		if(_projectName == null)
			_projectName = "Unknown";
	
		bw = new BufferedWriter(new FileWriter("VCProj/"+_projectName+".vcproj"));

		FileWriteln("<VisualStudioProject");
		FileWriteKeyValue("ProjectType", null, "Visual C++");
		FileWriteKeyValue("Version", sr.GetValue("VisualStudioProject", "Version"), "9.00");	
		FileWriteKeyValue("Name", null, _projectName);
		FileWriteKeyValue("ProjectGUID", null, "{54BB2D5D-2222-404C-A274-EF8E2D18014A}");
		FileWriteKeyValue("RootNamespace", null, _projectName);
		FileWriteKeyValue("TargetFrameworkVersion", null, "0");
		FileWriteln(">");
	
		FileWriteln("	<Platforms> <Platform Name=\"Win32\"/> </Platforms>");
		
		FileWriteln("	<Configurations>");
		
		Vector configs = sr.GetTagData("Configurations");
		
		for(int i=0;i<configs.size();i++)
		{
			String configName = (String)configs.get(i);
			
			String outputDirName = sr.GetValue(configName, "OutputDirectory");
			if(outputDirName == null)
				outputDirName = configName;
				
			FileWriteln("<Configuration");			
			FileWriteKeyValue("Name", null, configName);
			FileWriteKeyValue("OutputDirectory", null, outputDirName);
			FileWriteKeyValue("IntermediateDirectory", null, outputDirName);
			FileWriteKeyValue("ConfigurationType", null, "1");
			FileWriteKeyValue("InheritedPropertySheets", null, "$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC60.vsprops");
			FileWriteKeyValue("UseOfMFC", null, "0");
			FileWriteKeyValue("ATLMinimizesCRunTimeLibraryUsage", null, "false");
			FileWriteln(">");
			
			FileWriteln("<Tool");
				FileWriteKeyValue("Name", null, "VCCLCompilerTool");
				FileWriteKeyValue("Optimization", sr.GetValue(configName, "Optimization"), "0");
				FileWriteKeyValue("AdditionalIncludeDirectories", GetAdditionalIncludeDirs(), "");
				//FileWriteKeyValue("InlineFunctionExpansion", null, "2");
				FileWriteKeyValue("FavorSizeOrSpeed", null, "2");
				String preStr = sr.GetValue(configName, "PreprocessorDefinitions").trim();
				preStr = preStr.replace(" ", ";");
				FileWriteKeyValue("PreprocessorDefinitions", preStr, "WIN32;_WINDOWS");
				FileWriteKeyValue("MinimalRebuild", null, "true");
				FileWriteKeyValue("PrecompiledHeaderFile", null, outputDirName+"/"+_projectName+".pch");
				FileWriteKeyValue("AssemblerListingLocation", null, outputDirName+"/");
				FileWriteKeyValue("ObjectFile", null, outputDirName+"/");
				FileWriteKeyValue("ProgramDataBaseFileName", null, outputDirName+"/");
				FileWriteKeyValue("SuppressStartupBanner", null, "true");
				FileWriteKeyValue("RuntimeLibrary", sr.GetValue(configName, "RuntimeLibrary"), "1");				
				if(sr.GetValue(configName, "DebugInformationFormat")!=null)
					FileWriteKeyValue("DebugInformationFormat", sr.GetValue(configName, "DebugInformationFormat"), "");
					
			FileWriteln("/>");		


			
			FileWriteln("<Tool");
				FileWriteKeyValue("Name", null, "VCLinkerTool");
				FileWriteKeyValue("AdditionalDependencies", sr.GetValue(configName, "AdditionalDependencies"), "");
				FileWriteKeyValue("OutputFile", sr.GetValue(configName, "OutputFile"), outputDirName+"/"+_projectName+".exe");
				FileWriteKeyValue("ProgramDatabaseFile", null, outputDirName+"/"+_projectName+".pdb");
				FileWriteKeyValue("IgnoreDefaultLibraryNames", sr.GetValue(configName, "IgnoreDefaultLibraryNames"), "");
				
				if(configName.indexOf("debug") != -1 || configName.indexOf("Debug") != -1)
					FileWriteKeyValue("LinkIncremental", null, "2");
				else
					FileWriteKeyValue("LinkIncremental", null, "1");
				
				FileWriteKeyValue("SuppressStartupBanner", null, "true");
				FileWriteKeyValue("GenerateDebugInformation", null, "true");
				FileWriteKeyValue("SubSystem", null, "2");
				FileWriteKeyValue("RandomizedBaseAddress", null, "1");
				FileWriteKeyValue("DataExecutionPrevention", null, "0");
				FileWriteKeyValue("TargetMachine", null, "1");
				FileWriteKeyValue("AdditionalLibraryDirectories", null, "");
			FileWriteln("/>");			
			
			FileWriteln("</Configuration>");
		}
		
		FileWriteln("	</Configurations>");
		
		FileWriteln("<Files>");
		
		Vector vec = sr.GetTagData("IncludeFolders");
		for(int i=0;i<vec.size();i++)
			Convert(new File((String)vec.get(i)));
		
		FileWriteln("</Files>");
		
		FileWriteln("</VisualStudioProject>");
		bw.close();	
	}
	
	public void CreateSlnFile() throws Exception
	{
		bw = new BufferedWriter(new FileWriter("VCProj/"+_projectName+".sln"));
	
		FileWriteln("Microsoft Visual Studio Solution File, Format Version 10.00");
		FileWriteln("# Visual C++ Express 2008");
		FileWriteln("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \""+_projectName+"\", \""+_projectName+".vcproj\", \"{54BB2D5D-2222-404C-A274-EF8E2D18014A}\"");
		FileWriteln("EndProject");
		FileWriteln("Global");

		FileWriteln("GlobalSection(SolutionConfigurationPlatforms) = preSolution");

		Vector configsVec = sr.GetTagData("Configurations");

		for(int i=0;i<configsVec.size();i++)
		{
			String configName = (String	)configsVec.get(i);				
			FileWriteln(configName+" = "+configName);
		}

		FileWriteln("EndGlobalSection");
		FileWriteln("GlobalSection(ProjectConfigurationPlatforms) = postSolution");
		for(int i=0;i<configsVec.size();i++)
		{
			String configName = (String)configsVec.get(i);
			FileWriteln("{54BB2D5D-2222-404C-A274-EF8E2D18014A}."+configName+".ActiveCfg = "+configName);
			FileWriteln("{54BB2D5D-2222-404C-A274-EF8E2D18014A}."+configName+".Build.0 = "+configName);
		}
		FileWriteln("EndGlobalSection");

		FileWriteln("GlobalSection(SolutionProperties) = preSolution");
		FileWriteln("HideSolutionNode = FALSE");
		FileWriteln("EndGlobalSection");
		FileWriteln("EndGlobal");

		bw.close();	
	}
	
	
	public void Convert(File f) throws Exception
	{
		if(IsInExludeFolders(f))
			return;
	
		PrintStrWithTabs(numTabs, "<Filter Name=\""+f.getName()+"\">");
		numTabs++;
		
		File files[]=f.listFiles();
		Vector v = new Vector();

		for(int i=0;i<files.length;i++)
		{
			String filePath = files[i].getCanonicalPath();
			if(files[i].isDirectory())
				v.add(files[i]);
			else if(IsInExludeFiles(files[i]) == false)
				PrintStrWithTabs(numTabs, "<File RelativePath=\""+filePath+"\"/>");
		}
		
		for(int i=0;i<v.size();i++)
		{
			File folder = (File)v.get(i);
			if(folder.getName().equalsIgnoreCase(".svn") == false)
				Convert(folder);
		}
		
		numTabs--;
		PrintStrWithTabs(numTabs, "</Filter>");
	}
	
	String GetAdditionalIncludeDirs() throws Exception
	{
		additionalDirs.removeAllElements();

		Vector vec = sr.GetTagData("IncludeFolders");
		
		for(int i=0;i<vec.size();i++)
			AddAllFolders(new File((String)vec.get(i)));
		
		String str = "";
		
		for(int i=0;i<additionalDirs.size();i++)
		{
			if(i!=0)
				str += ";";
			str += (String)additionalDirs.get(i);
		}
		
		return str;		
	}
	
	
	void AddAllFolders(File f) throws Exception
	{
		if(IsInExludeFolders(f) == false)
			additionalDirs.add(f.getCanonicalPath());
		
		File files[]=f.listFiles();

		for(int i=0;i<files.length;i++)
		{
			if(files[i].isDirectory() && files[i].getName().equalsIgnoreCase(".svn") == false)
				AddAllFolders(files[i]);
		}	
	}

	boolean IsInExludeFolders(File f)
	{
		if(excludeFoldersVec != null)
		{
			String fileName = f.getAbsolutePath();
			
			for(int i=0; i<excludeFoldersVec.size(); i++)
			{
				if(((String)excludeFoldersVec.get(i)).equalsIgnoreCase(fileName))
					return true;
			}
		}
		return false;
	}
	
	boolean IsInExludeFiles(File f)
	{
		if(excludeFilesVec != null)
		{
			String fileName = f.getAbsolutePath();
			
			for(int i=0; i<excludeFilesVec.size(); i++)
			{
				if(((String)excludeFilesVec.get(i)).equalsIgnoreCase(fileName))
					return true;
			}
		}
		return false;
	}
	
	void PrintStrWithTabs(int numTabs, String str)throws Exception
	{
		String tabStr="";
		for(int i=0;i<numTabs;i++)
			tabStr += "\t";

		bw.write(tabStr,0,tabStr.length());
		bw.flush();

		bw.write(str,0,str.length());
		bw.flush();
		bw.newLine();
	}
	
	void FileWriteKeyValue(String key, String value, String defaultVal)throws Exception
	{
		if(value!=null)
			FileWriteln(key+"=\""+value+"\"");
		else
			FileWriteln(key+"=\""+defaultVal+"\"");
	}

	void FileWriteln(String str)throws Exception
	{
		bw.write(str,0,str.length());
		bw.flush();
		bw.newLine();
	}
	
}


class ScriptReader
{
	Vector tagNames = new Vector();
	Vector tagData = new Vector();
	
	BufferedReader br;
	File scriptFile;
	
	public ScriptReader(String filePath)throws Exception
	{
		br = new BufferedReader(new FileReader(filePath));
		scriptFile = new File(filePath);
		
		String prevLine = null;
		String line = null;
		
		while( (line=br.readLine()) != null)
		{
			if(line.trim().startsWith("//"))
				continue;

			if(line.trim().equals("{"))
				ReadTag(prevLine, br);

			prevLine = line;
		}
		FindAndReplace();
	}

	void FindAndReplace() throws Exception
	{
		Vector keys = GetKeys("ReplacePaths");
		Vector values = GetValues("ReplacePaths");

		int size = keys.size();
		if(size > values.size())
			size = values.size();

		for(int i=0;i<size;i++)
		{
			String keyStr = "<"+(String)keys.get(i)+">";
			String valueStr = (String)values.get(i);
			valueStr = valueStr.substring(1, valueStr.length()-1);
			
			ReplaceValues(keyStr, valueStr);
		}	
	}
	
	void ReplaceValues(String findKey, String replaceValue) throws Exception
	{
		for(int i=0;i<tagData.size();i++)
		{
			Vector singleTagData = (Vector)tagData.get(i);
		
			for(int j=0;j<singleTagData.size();j++)
			{
				String lineData = (String)singleTagData.get(j);
				
				if(lineData.indexOf(findKey) != -1)
				{
					lineData = lineData.replaceAll(findKey, replaceValue);
					singleTagData.setElementAt(lineData, j);
				}
			}
		}
	}

	void ReadTag(String tagName, BufferedReader br)throws Exception
	{
		tagNames.add(tagName);
		
		Vector data = new Vector();
		
		String line = null;
		while( (line = br.readLine().trim()) != null)
		{
			if(line.length() == 0 || line.trim().startsWith("#"))
				continue;
				
			if(line.trim().equals("}"))
				break;

			if(line.indexOf("=") == -1)
			{
				String[] tokens = line.split(" ");
				for(int i=0;i<tokens.length;i++)
					data.add(tokens[i]);
			}
			else
				data.add(line);
		}
		tagData.add(data);
	}
	
	Vector GetTagData(String findTag)throws Exception
	{
		findTag = findTag.trim();
		
		for(int i=0;i<tagNames.size();i++)
		{
			String tagName = (String)tagNames.get(i);
			if(tagName.equalsIgnoreCase(findTag))
			{
				return (Vector)tagData.get(i);
			}
		}
		return null;
	}
	
	String GetValue(String findTag, String key)throws Exception
	{
		findTag = findTag.trim();
		
		Vector linesVec = GetTagData(findTag);
		
		if(linesVec != null)
		{
			for(int j=0;j<linesVec.size();j++)
			{
				String str = (String)linesVec.get(j);
				if(str.indexOf("=") == -1)
					continue;
				
				String tokens[] = str.split("=");
				
				if(tokens.length == 2 && tokens[0].trim().equalsIgnoreCase(key))
				{
					String valueStr = tokens[1].trim();
					char ch = '\"';
					if(valueStr.charAt(0) == ch && valueStr.charAt(valueStr.length()-1) == ch)
					{
						valueStr = valueStr.substring(1,valueStr.length()-1);
						return valueStr;
					}
					else
					{
						System.out.println("\n\nParse Error\n\n");
						System.exit(0);
					}
				}
			}
		}
		
		return null;
	}
	
	Vector GetKeys(String findTag)throws Exception
	{
		findTag = findTag.trim();
		Vector keysVector = new Vector();
		
		Vector linesVec = GetTagData(findTag);
		if(linesVec != null)
		{
			for(int j=0;j<linesVec.size();j++)
			{
				String str = (String)linesVec.get(j);
				if(str.indexOf("=") == -1)
					continue;
				
				String tokens[] = str.split("=");
				
				if(tokens.length == 2)
				{
					keysVector.add(tokens[0].trim());
				}
			}
		}
		
		return keysVector;
	}
	
	Vector GetValues(String findTag)throws Exception
	{
		findTag = findTag.trim();
		Vector valuesVector = new Vector();
		
		Vector linesVec = GetTagData(findTag);
		if(linesVec != null)
		{
			for(int j=0;j<linesVec.size();j++)
			{
				String str = (String)linesVec.get(j);
				if(str.indexOf("=") == -1)
					continue;
				
				String tokens[] = str.split("=");
				
				if(tokens.length == 2)
				{
					valuesVector.add(tokens[1].trim());
				}
			}
		}
		
		return valuesVector;
	}		
}


/*

<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="TestTool"
	ProjectGUID="{54BB2D5D-2222-404C-A274-EF8E2D18014A}"
	RootNamespace="TestTool"
	TargetFrameworkVersion="0">
	
	<Platforms> <Platform Name="Win32"/> </Platforms>

	<Configurations>
		<Configuration
			Name="Release|Win32"
			OutputDirectory=".\__Release"
			IntermediateDirectory=".\__Release"
			ConfigurationType="1"
			InheritedPropertySheets="$(VCInstallDir)VCProjectDefaults\UpgradeFromVC60.vsprops"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="false"
			>
				
			<Tool
				Name="VCCLCompilerTool"
				Optimization="4"
				InlineFunctionExpansion="2"
				FavorSizeOrSpeed="2"
				AdditionalIncludeDirectories=""
				PreprocessorDefinitions="WIN32;NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN"
				RuntimeLibrary="2"
				PrecompiledHeaderFile=".\__Release/TestTool.pch"
				AssemblerListingLocation=".\__Release/"
				ObjectFile=".\Release/"
				ProgramDataBaseFileName=".\__Release/"
				SuppressStartupBanner="true"
			/>
			
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="glu32.lib opengl32.lib comctl32.lib wsock32.lib ../FLTK_LIBS/*.lib"
				OutputFile="TestTool.exe"
				LinkIncremental="1"
				SuppressStartupBanner="true"
				AdditionalLibraryDirectories=""
				IgnoreDefaultLibraryNames="libcd"
				ProgramDatabaseFile=".\__Release/TestTool.pdb"
				SubSystem="2"
				RandomizedBaseAddress="1"
				DataExecutionPrevention="0"
				TargetMachine="1"
			/>
		</Configuration>
		
		
		<Configuration Name="Debug|Win32"
				OutputDirectory=".\__Debug"
				IntermediateDirectory=".\__Debug"
				ConfigurationType="1"
				InheritedPropertySheets="$(VCInstallDir)VCProjectDefaults\UpgradeFromVC60.vsprops"
				UseOfMFC="0"
				ATLMinimizesCRunTimeLibraryUsage="false"
				>

			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				AdditionalIncludeDirectories=""
				PreprocessorDefinitions="WIN32;_DEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;VC_EXTRA_LEAN;WIN32_EXTRA_LEAN"
				MinimalRebuild="true"
				RuntimeLibrary="3"
				PrecompiledHeaderFile=".\__Debug/TestTool.pch"
				AssemblerListingLocation=".\__Debug/"
				ObjectFile=".\__Debug/"
				ProgramDataBaseFileName=".\__Debug/"
				SuppressStartupBanner="true"
				DebugInformationFormat="4"
			/>
			
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="glu32.lib opengl32.lib comctl32.lib wsock32.lib ../FLTK_LIBS/*.lib"
				OutputFile="./__Debug/TestTool.exe"
				LinkIncremental="2"
				SuppressStartupBanner="true"
				AdditionalLibraryDirectories=""
				IgnoreDefaultLibraryNames="libcd"
				GenerateDebugInformation="true"
				ProgramDatabaseFile=".\__Debug/TestTool.pdb"
				SubSystem="2"
				RandomizedBaseAddress="1"
				DataExecutionPrevention="0"
				TargetMachine="1"
			/>
		</Configuration>
		
	</Configurations>

	<Files>
	</Files>

</VisualStudioProject>
*/

