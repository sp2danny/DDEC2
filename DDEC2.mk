##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=DDEC2
ConfigurationName      :=Debug
WorkspacePath          := "/home/sp2danny/extra/DDEC2"
ProjectPath            := "/home/sp2danny/extra/DDEC2"
IntermediateDirectory  :=./bin/Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Daniel Nyström
Date                   :=02/06/19
CodeLitePath           :="/home/sp2danny/.codelite"
LinkerName             :=/usr/bin/clang++ 
SharedObjectLinkerName :=/usr/bin/clang++ -shared -fPIC
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
OutputFile             :=$(ProjectName)_dbg.out
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="DDEC2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system $(LibrarySwitch)boost_filesystem 
ArLibs                 :=  "boost_system" "boost_filesystem" 
LibPath                := $(LibraryPathSwitch)/usr/lib/boost 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++ 
CC       := /usr/bin/clang 
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Wno-multichar -g -O0 $(Preprocessors)
CFLAGS   :=  -g -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
Objects0=$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./bin/Debug || $(MakeDirCommand) ./bin/Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix): src/Crypt.cpp $(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Crypt.cpp$(DependSuffix) -MM "src/Crypt.cpp"

$(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix): src/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Crypt.cpp$(PreprocessSuffix) "src/Crypt.cpp"

$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix): src/bitmap.cpp $(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix) -MM "src/bitmap.cpp"

$(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix) "src/bitmap.cpp"

$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp $(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM "src/bitstream.cpp"

$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) "src/bitstream.cpp"

$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix): src/fbct2.cpp $(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/fbct2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix) -MM "src/fbct2.cpp"

$(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix) "src/fbct2.cpp"

$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix): src/frame.cpp $(IntermediateDirectory)/src_frame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/frame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_frame.cpp$(DependSuffix): src/frame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_frame.cpp$(DependSuffix) -MM "src/frame.cpp"

$(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix): src/frame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_frame.cpp$(PreprocessSuffix) "src/frame.cpp"

$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix): src/lzv.cpp $(IntermediateDirectory)/src_lzv.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/lzv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_lzv.cpp$(DependSuffix): src/lzv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_lzv.cpp$(DependSuffix) -MM "src/lzv.cpp"

$(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix): src/lzv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_lzv.cpp$(PreprocessSuffix) "src/lzv.cpp"

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./bin/Debug/*$(ObjectSuffix)
	$(RM) ./bin/Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/DDEC2"


