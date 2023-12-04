##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=DDEC2
ConfigurationName      :=Release
WorkspacePath          :=/home/daniel/project/ddec2
ProjectPath            :=/home/daniel/project/ddec2
IntermediateDirectory  :=./bin/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=daniel
Date                   :=04/12/23
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
ObjectsFileList        :="DDEC2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -std=c++2b 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := 

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
Objects0=$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/fft_pffft.c$(ObjectSuffix) $(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_wave.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix): src/lzv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_lzv.cpp$(DependSuffix) -MM src/lzv.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/lzv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix): src/lzv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix) src/lzv.cpp

$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix): src/frame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_frame.cpp$(DependSuffix) -MM src/frame.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/frame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix): src/frame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix) src/frame.cpp

$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM src/bitstream.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) src/bitstream.cpp

$(IntermediateDirectory)/fft_pffft.c$(ObjectSuffix): fft/pffft.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fft_pffft.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fft_pffft.c$(DependSuffix) -MM fft/pffft.c
	$(CC) $(SourceSwitch) "/home/daniel/project/ddec2/fft/pffft.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fft_pffft.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fft_pffft.c$(PreprocessSuffix): fft/pffft.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fft_pffft.c$(PreprocessSuffix) fft/pffft.c

$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix) -MM src/fbct2.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/fbct2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix): src/fbct2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix) src/fbct2.cpp

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) src/main.cpp

$(IntermediateDirectory)/src_wave.cpp$(ObjectSuffix): src/wave.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_wave.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_wave.cpp$(DependSuffix) -MM src/wave.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/wave.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wave.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wave.cpp$(PreprocessSuffix): src/wave.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wave.cpp$(PreprocessSuffix) src/wave.cpp

$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix) -MM src/bitmap.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix): src/bitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix) src/bitmap.cpp

$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix) -MM src/Crypt.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/daniel/project/ddec2/src/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix): src/Crypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix) src/Crypt.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/Release/


