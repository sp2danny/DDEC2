##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=player
ConfigurationName      :=Release
WorkspacePath          := "/home/sp2danny/extra/DDEC2"
ProjectPath            := "/home/sp2danny/extra/DDEC2"
IntermediateDirectory  :=./bin/Release
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
OutputFile             :=$(ProjectName).out
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="player.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -std=c++17 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/usr/include/gtk-2.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/gtk-2.0/include $(IncludeSwitch)/usr/include/gdk-pixbuf-2.0 $(IncludeSwitch)/usr/include/atk-1.0 $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gtk-x11-2.0 $(LibrarySwitch)gdk-x11-2.0 $(LibrarySwitch)pangocairo-1.0 $(LibrarySwitch)atk-1.0 $(LibrarySwitch)cairo $(LibrarySwitch)gdk_pixbuf-2.0 $(LibrarySwitch)gio-2.0 $(LibrarySwitch)pangoft2-1.0 $(LibrarySwitch)pango-1.0 $(LibrarySwitch)gobject-2.0 $(LibrarySwitch)glib-2.0 $(LibrarySwitch)fontconfig $(LibrarySwitch)freetype $(LibrarySwitch)boost_system $(LibrarySwitch)pthread $(LibrarySwitch)boost_system $(LibrarySwitch)boost_filesystem $(LibrarySwitch)boost_thread $(LibrarySwitch)boost_chrono $(LibrarySwitch)gmp $(LibrarySwitch)gmpxx $(LibrarySwitch)avcodec $(LibrarySwitch)avformat $(LibrarySwitch)avutil $(LibrarySwitch)swresample 
ArLibs                 :=  "gtk-x11-2.0" "gdk-x11-2.0" "pangocairo-1.0" "atk-1.0" "cairo" "gdk_pixbuf-2.0" "gio-2.0" "pangoft2-1.0" "pango-1.0" "gobject-2.0" "glib-2.0" "fontconfig" "freetype" "boost_system" "pthread" "boost_system" "boost_filesystem" "boost_thread" "boost_chrono" "gmp" "gmpxx" "avcodec" "avformat" "avutil" "swresample" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++ 
CC       := /usr/bin/clang 
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Wno-multichar -O2 $(Preprocessors)
CFLAGS   :=  -O2 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
Objects0=$(IntermediateDirectory)/src_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_frame.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_lzv.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) 



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
	@test -d ./bin/Release || $(MakeDirCommand) ./bin/Release

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

$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix): src/bitstream.cpp $(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/bitstream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitstream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitstream.cpp$(DependSuffix) -MM "src/bitstream.cpp"

$(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix): src/bitstream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitstream.cpp$(PreprocessSuffix) "src/bitstream.cpp"

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

$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix): src/nibblemap.cpp $(IntermediateDirectory)/src_nibblemap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/nibblemap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_nibblemap.cpp$(DependSuffix): src/nibblemap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_nibblemap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_nibblemap.cpp$(DependSuffix) -MM "src/nibblemap.cpp"

$(IntermediateDirectory)/src_nibblemap.cpp$(PreprocessSuffix): src/nibblemap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_nibblemap.cpp$(PreprocessSuffix) "src/nibblemap.cpp"

$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix): src/player.cpp $(IntermediateDirectory)/src_player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_player.cpp$(DependSuffix): src/player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_player.cpp$(DependSuffix) -MM "src/player.cpp"

$(IntermediateDirectory)/src_player.cpp$(PreprocessSuffix): src/player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_player.cpp$(PreprocessSuffix) "src/player.cpp"

$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix): src/bitmap.cpp $(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitmap.cpp$(DependSuffix) -MM "src/bitmap.cpp"

$(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix): src/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitmap.cpp$(PreprocessSuffix) "src/bitmap.cpp"

$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix): src/fbct2.cpp $(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sp2danny/extra/DDEC2/src/fbct2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fbct2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fbct2.cpp$(DependSuffix) -MM "src/fbct2.cpp"

$(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix): src/fbct2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fbct2.cpp$(PreprocessSuffix) "src/fbct2.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./bin/Release/*$(ObjectSuffix)
	$(RM) ./bin/Release/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-release/player"


