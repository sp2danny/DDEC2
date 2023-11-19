##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=ce
ConfigurationName      :=Release
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=19/11/23
CodeLitePath           :=/home/daniel/.codelite
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
OutputFile             :=$(ProjectName).out
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ce.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -std=c++2b 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++-15
CC       := /usr/bin/clang-15
CXXFLAGS := -std=c++2b -Wall -Wextra -Werror -pedantic -Wno-multichar -Wno-pre-c++2b-compat -O3 $(Preprocessors)
CFLAGS   :=  -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ce.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./bin/Release || $(MakeDirCommand) ./bin/Release


$(IntermediateDirectory)/.d:
	@test -d ./bin/Release || $(MakeDirCommand) ./bin/Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix) -MM src/Crypt.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix): src/Crypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix) src/Crypt.cpp

$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM src/bitstream.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) src/bitstream.cpp

$(IntermediateDirectory)/src_ce.cpp$(ObjectSuffix): src/ce.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ce.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ce.cpp$(DependSuffix) -MM src/ce.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/ce.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ce.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ce.cpp$(PreprocessSuffix): src/ce.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ce.cpp$(PreprocessSuffix) src/ce.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/Release/


