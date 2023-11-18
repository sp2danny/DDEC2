##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=patest
ConfigurationName      :=Release
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=11/11/23
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
ObjectsFileList        :="patest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -std=c++2b 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../portaudio/include $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)portaudio $(LibrarySwitch)rt $(LibrarySwitch)m $(LibrarySwitch)asound $(LibrarySwitch)jack 
ArLibs                 :=  "portaudio" "rt" "m" "asound" "jack" 
LibPath                := $(LibraryPathSwitch)bin 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++-15
CC       := /usr/bin/clang-15
CXXFLAGS := -std=c++2b -Wall -Wextra -Werror -pedantic -Wno-multichar -Wno-pre-c++2b-compat -pthread -O3 $(Preprocessors)
CFLAGS   :=  -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_patest.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/src_patest.cpp$(ObjectSuffix): src/patest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_patest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_patest.cpp$(DependSuffix) -MM src/patest.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/patest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_patest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_patest.cpp$(PreprocessSuffix): src/patest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_patest.cpp$(PreprocessSuffix) src/patest.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/Release/


