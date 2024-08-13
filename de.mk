##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=de
ConfigurationName      :=Release
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=13/08/24
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
ObjectsFileList        :="de.txt"
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
CXXFLAGS := -std=c++2b -fmodules -Wall -Wextra -Werror -pedantic -Wno-multichar -Wno-pre-c++2b-compat -O3 $(Preprocessors)
CFLAGS   :=  -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_lz.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_de.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM src/bitstream.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) src/bitstream.cpp

$(IntermediateDirectory)/src_lz.cpp$(ObjectSuffix): src/lz.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_lz.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_lz.cpp$(DependSuffix) -MM src/lz.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/lz.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_lz.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_lz.cpp$(PreprocessSuffix): src/lz.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_lz.cpp$(PreprocessSuffix) src/lz.cpp

$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix) -MM src/Crypt.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix): src/Crypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix) src/Crypt.cpp

$(IntermediateDirectory)/src_de.cpp$(ObjectSuffix): src/de.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_de.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_de.cpp$(DependSuffix) -MM src/de.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/de.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_de.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_de.cpp$(PreprocessSuffix): src/de.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_de.cpp$(PreprocessSuffix) src/de.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/Release/


