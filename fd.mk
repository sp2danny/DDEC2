##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=fd
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Debug
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=04/11/24
CodeLitePath           :=/home/daniel/.codelite
MakeDirCommand         :=mkdir -p
LinkerName             :=/usr/bin/clang++-15
SharedObjectLinkerName :=/usr/bin/clang++-15 -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=.
OutputFile             :=./$(ProjectName)_dbg.out
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            := -std=c++2b 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++-15
CC       := /usr/bin/clang-15
CXXFLAGS := -std=c++2b -fmodules -Wall -Wextra -Werror -pedantic -Wno-multichar -Wno-pre-c++2b-compat -g -O0 $(Preprocessors)
CFLAGS   :=  -g -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_fod.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_mersenne-twister.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_util.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_fd.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp $(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM src/bitstream.cpp

$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) src/bitstream.cpp

$(IntermediateDirectory)/src_fod.cpp$(ObjectSuffix): src/fod.cpp $(IntermediateDirectory)/src_fod.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/fod.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fod.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fod.cpp$(DependSuffix): src/fod.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fod.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fod.cpp$(DependSuffix) -MM src/fod.cpp

$(IntermediateDirectory)/src_fod.cpp$(PreprocessSuffix): src/fod.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fod.cpp$(PreprocessSuffix) src/fod.cpp

$(IntermediateDirectory)/src_mersenne-twister.cpp$(ObjectSuffix): src/mersenne-twister.cpp $(IntermediateDirectory)/src_mersenne-twister.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/mersenne-twister.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_mersenne-twister.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_mersenne-twister.cpp$(DependSuffix): src/mersenne-twister.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_mersenne-twister.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_mersenne-twister.cpp$(DependSuffix) -MM src/mersenne-twister.cpp

$(IntermediateDirectory)/src_mersenne-twister.cpp$(PreprocessSuffix): src/mersenne-twister.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_mersenne-twister.cpp$(PreprocessSuffix) src/mersenne-twister.cpp

$(IntermediateDirectory)/src_util.cpp$(ObjectSuffix): src/util.cpp $(IntermediateDirectory)/src_util.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/util.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_util.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_util.cpp$(DependSuffix): src/util.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_util.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_util.cpp$(DependSuffix) -MM src/util.cpp

$(IntermediateDirectory)/src_util.cpp$(PreprocessSuffix): src/util.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_util.cpp$(PreprocessSuffix) src/util.cpp

$(IntermediateDirectory)/src_fd.cpp$(ObjectSuffix): src/fd.cpp $(IntermediateDirectory)/src_fd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/fd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fd.cpp$(DependSuffix): src/fd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fd.cpp$(DependSuffix) -MM src/fd.cpp

$(IntermediateDirectory)/src_fd.cpp$(PreprocessSuffix): src/fd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fd.cpp$(PreprocessSuffix) src/fd.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


