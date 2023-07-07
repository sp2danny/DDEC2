##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=player
ConfigurationName      :=Release
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=07/07/23
CodeLitePath           :=/home/daniel/.codelite
LinkerName             :=/usr/bin/clang++-14
SharedObjectLinkerName :=/usr/bin/clang++-14 -shared -fPIC
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
ObjectsFileList        :="player.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -std=c++2a 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)sfml-graphics $(LibrarySwitch)sfml-window $(LibrarySwitch)sfml-system 
ArLibs                 :=  "sfml-graphics" "sfml-window" "sfml-system" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++-14
CC       := /usr/bin/clang-14
CXXFLAGS := -std=c++2a -Wall -Wextra -Werror -Wno-multichar -O2 $(Preprocessors)
CFLAGS   :=  -O2 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix) -MM src/fbct2.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/fbct2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix): src/fbct2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix) src/fbct2.cpp

$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix): src/lzv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_lzv.cpp$(DependSuffix) -MM src/lzv.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/lzv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix): src/lzv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix) src/lzv.cpp

$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM src/bitstream.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) src/bitstream.cpp

$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix): src/player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_player.cpp$(DependSuffix) -MM src/player.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_player.cpp$(PreprocessSuffix): src/player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_player.cpp$(PreprocessSuffix) src/player.cpp

$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix): src/frame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_frame.cpp$(DependSuffix) -MM src/frame.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/frame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix): src/frame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix) src/frame.cpp

$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix): src/nibblemap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_nibblemap.cpp$(DependSuffix) -MM src/nibblemap.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/nibblemap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_nibblemap.cpp$(PreprocessSuffix): src/nibblemap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_nibblemap.cpp$(PreprocessSuffix) src/nibblemap.cpp

$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix) -MM src/Crypt.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix): src/Crypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix) src/Crypt.cpp

$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix) -MM src/bitmap.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix): src/bitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix) src/bitmap.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/Release/


