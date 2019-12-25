//
// "$Id$"
//
// Hello, World! program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// TODO: make sure that there are no formatting characters in any of the path names (%s,...)
// TODO: make sure that the FLTK path exists, maybe even check for known files
// TODO: handle all possible errors when writing files (if ( && && && ) else ...)
// TODO: we can read a lot of information from the CMake files in the FLTK tree!

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <FL/Fl_File_Chooser.H>

#include "flink_ui.h"


void showAboutWindow()
{
  fl_message("%s",
             "Flink creates all files needed to compile FLTK for Android.\n\n"
             "Flink was written for FLTK 1.4 and tested with\n"
             "AndroidStudio 3.5 ."
             );
}


void selectSourceFolder()
{
  const char *dir = fl_dir_chooser("Select the FLTK root folder", wSourceFolder->value(), 0);
  if (dir) {
    wSourceFolder->value(dir);
  }
}


void selectProjectFolder()
{
  char oldDir[FL_PATH_MAX];
  fl_getcwd(oldDir, FL_PATH_MAX);
  fl_chdir(wSourceFolder->value());
  const char *dir = fl_dir_chooser("Select the AndroidStudio subfolder", wProjectFolder->value(), 1);
  fl_chdir(oldDir);
  if (dir) {
    wProjectFolder->value(dir);
  }
}


void createFile(const char *dir, const char *name, const char *text)
{
  char filename[FL_PATH_MAX];
  strcpy(filename, dir);
  strcat(filename, name);
  fl_make_path_for_file(filename);
  FILE *f = fl_fopen(filename, "wb");
  fwrite(text, strlen(text), 1, f);
  fclose(f);
}


void createFile(const char *dir, const char *name, const unsigned char *data, size_t size)
{
  char filename[FL_PATH_MAX];
  strcpy(filename, dir);
  strcat(filename, name);
  fl_make_path_for_file(filename);
  FILE *f = fl_fopen(filename, "wb");
  fwrite(data, size, 1, f);
  fclose(f);
}


void createProjectFolder()
{
  char projectDir[FL_PATH_MAX];
  fl_chdir(wSourceFolder->value());
  fl_filename_absolute(projectDir, FL_PATH_MAX, wProjectFolder->value());
  int n = strlen(projectDir);
  if (projectDir[n]!='/') strcat(projectDir, "/");
  fl_make_path(projectDir);

  //fl_message("Can't create project. Not yet implemented.");

  // create this for the entire project
  createFile(projectDir, "FL/abi-version.h", "/* #undef FL_ABI_VERSION */\n");
  createFile(projectDir, "build.gradle",
             "buildscript {\n"
             "    repositories {\n"
             "        jcenter()\n"
             "        google()\n"
             "    }\n"
             "    dependencies {\n"
             "        classpath 'com.android.tools.build:gradle:3.5.3'\n"
             "    }\n"
             "}\n\n"
             "allprojects {\n"
             "    repositories {\n"
             "        jcenter()\n"
             "        google()\n"
             "    }\n"
             "}\n");
  // append directives for every library and app
  createFile(projectDir, "settings.gradle",
             "include ':fltk'\n"
             "include ':adjuster'\n");
  createFile(projectDir, "config.h",
             "#define FLTK_DATADIR \"/usr/local/share/fltk\"\n"
             "#define FLTK_DOCDIR \"/usr/local/share/doc/fltk\"\n"
             "#define BORDER_WIDTH 2\n"
             "#define HAVE_GL 0\n"
             "#define HAVE_GL_GLU_H 0\n"
             "/* #undef HAVE_GLXGETPROCADDRESSARB */\n"
             "#define USE_COLORMAP 1\n"
             "#define HAVE_XINERAMA 0\n"
             "#define USE_XFT 0\n"
             "#define USE_PANGO 0\n"
             "#define HAVE_XDBE 0\n"
             "#define USE_XDBE HAVE_XDBE\n"
             "#define HAVE_XFIXES 0\n"
             "#define HAVE_XCURSOR 0\n"
             "#define HAVE_XRENDER 0\n"
             "#define HAVE_X11_XREGION_H 0\n"
             "/* #undef __APPLE_QUARTZ__ */\n"
             "/* #undef USE_X11 */\n"
             "/* #undef USE_SDL */\n"
             "#define HAVE_OVERLAY 0\n"
             "#define HAVE_GL_OVERLAY HAVE_OVERLAY\n"
             "#define WORDS_BIGENDIAN 0\n"
             "#define U16 unsigned short\n"
             "#define U32 unsigned\n"
             "#define U64 unsigned long\n"
             "#define HAVE_DIRENT_H 1\n"
             "#define HAVE_SCANDIR 1\n"
             "#define HAVE_SCANDIR_POSIX 1\n"
             "#define HAVE_VSNPRINTF 1\n"
             "#define HAVE_SNPRINTF 1\n"
             "#define HAVE_STRINGS_H 1\n"
             "#define HAVE_STRCASECMP 1\n"
             "#define HAVE_STRLCAT 1\n"
             "#define HAVE_STRLCPY 1\n"
             "#define HAVE_LOCALE_H 1\n"
             "#define HAVE_LOCALECONV 1\n"
             "#define HAVE_SYS_SELECT_H 1\n"
             "/* #undef HAVE_SYS_STDTYPES_H */\n"
             "#define USE_POLL 0\n"
             "#define HAVE_LIBPNG 1\n"
             "#define HAVE_LIBZ 1\n"
             "#define HAVE_LIBJPEG 1\n"
             "/* #undef FLTK_USE_CAIRO */\n"
             "/* #undef FLTK_HAVE_CAIRO */\n"
             "#define HAVE_PNG_H 1\n"
             "/* #undef HAVE_LIBPNG_PNG_H */\n"
             "#define HAVE_PNG_GET_VALID 1\n"
             "#define HAVE_PNG_SET_TRNS_TO_ALPHA 1\n"
             "#define FLTK_USE_NANOSVG 1\n"
             "#define HAVE_PTHREAD 1\n"
             "#define HAVE_PTHREAD_H 1\n"
             "/* #undef HAVE_ALSA_ASOUNDLIB_H */\n"
             "#define HAVE_LONG_LONG 1\n"
             "#define FLTK_LLFMT \"%lld\"\n"
             "#define FLTK_LLCAST (long long)\n"
             "#define HAVE_DLFCN_H 1\n"
             "#define HAVE_DLSYM 1\n"
             "#define FL_NO_PRINT_SUPPORT 1\n"
             "/* #undef FL_CFG_NO_FILESYSTEM_SUPPORT */\n");

  // create this for every library
  createFile(projectDir, "fltk/build.gradle",
             "apply plugin: 'com.android.library'\n\n"
             "android {\n"
             "  compileSdkVersion 26\n"
             "  defaultConfig {\n"
             "    minSdkVersion 14\n"
             "    targetSdkVersion 26\n"
             "    externalNativeBuild {\n"
             "      cmake {\n"
             "        arguments '-DANDROID_STL=c++_shared'\n"
             "        targets 'fltk'\n"
             "      }\n"
             "    }\n"
             "  }\n"
             "  buildTypes {\n"
             "    release {\n"
             "      minifyEnabled false\n"
             "    }\n"
             "  }\n"
             "  externalNativeBuild {\n"
             "    cmake {\n"
             "      path 'src/main/cpp/CMakeLists.txt'\n"
             "    }\n"
             "  }\n"
             "}\n");
  createFile(projectDir, "fltk/src/main/AndroidManifest.xml",
             "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
             "          package=\"org.fltk.fltk\">\n"
             "</manifest>\n");
  createFile(projectDir, "fltk/src/main/cpp/CMakeLists.txt",
             "cmake_minimum_required(VERSION 3.6)\n"
             "\n"
             "set(CMAKE_VERBOSE_MAKEFILE on)\n"
             "\n"
             "set(FLTK_DIR \"/Users/matt/dev/fltk-1.4.git\")\n"
             "set(FLTK_IDE_DIR \"../../../..\")\n"
             "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=c++11\")\n"
             "\n"
             "function(list_transform_prepend var prefix)\n"
             "    set(temp \"\")\n"
             "    foreach(f ${${var}})\n"
             "        list(APPEND temp \"${prefix}${f}\")\n"
             "    endforeach()\n"
             "    set(${var} \"${temp}\" PARENT_SCOPE)\n"
             "endfunction()\n"
             "\n"
             "set (CPPFILES\n"
             "  Fl.cxx\n"
             "  Fl_Adjuster.cxx\n"
             "  Fl_Bitmap.cxx\n"
             "  Fl_Browser.cxx\n"
             "  Fl_Browser_.cxx\n"
             "  Fl_Browser_load.cxx\n"
             "  Fl_Box.cxx\n"
             "  Fl_Button.cxx\n"
             "  Fl_Chart.cxx\n"
             "  Fl_Check_Browser.cxx\n"
             "  Fl_Check_Button.cxx\n"
             "  Fl_Choice.cxx\n"
             "  Fl_Clock.cxx\n"
             "  Fl_Color_Chooser.cxx\n"
             "  Fl_Copy_Surface.cxx\n"
             "  Fl_Counter.cxx\n"
             "  Fl_Device.cxx\n"
             "  Fl_Dial.cxx\n"
             "  Fl_Help_Dialog_Dox.cxx\n"
             "  Fl_Double_Window.cxx\n"
             "  Fl_File_Browser.cxx\n"
             "  Fl_File_Chooser.cxx\n"
             "  Fl_File_Chooser2.cxx\n"
             "  Fl_File_Icon.cxx\n"
             "  Fl_File_Input.cxx\n"
             "  Fl_Graphics_Driver.cxx\n"
             "  Fl_Group.cxx\n"
             "  Fl_Help_View.cxx\n"
             "  Fl_Image.cxx\n"
             "  Fl_Image_Surface.cxx\n"
             "  Fl_Input.cxx\n"
             "  Fl_Input_.cxx\n"
             "  Fl_Input_Choice.cxx\n"
             "  Fl_Light_Button.cxx\n"
             "  Fl_Menu.cxx\n"
             "  Fl_Menu_.cxx\n"
             "  Fl_Menu_Bar.cxx\n"
             "  Fl_Menu_Button.cxx\n"
             "  Fl_Menu_Window.cxx\n"
             "  Fl_Menu_add.cxx\n"
             "  Fl_Menu_global.cxx\n"
             "  Fl_Multi_Label.cxx\n"
             "  Fl_Native_File_Chooser.cxx\n"
             "  Fl_Overlay_Window.cxx\n"
             "  Fl_Pack.cxx\n"
             "  Fl_Paged_Device.cxx\n"
             "  Fl_Pixmap.cxx\n"
             "  Fl_Positioner.cxx\n"
             "  Fl_Preferences.cxx\n"
             "  Fl_Printer.cxx\n"
             "  Fl_Progress.cxx\n"
             "  Fl_Repeat_Button.cxx\n"
             "  Fl_Return_Button.cxx\n"
             "  Fl_Roller.cxx\n"
             "  Fl_Round_Button.cxx\n"
             "  Fl_Screen_Driver.cxx\n"
             "  Fl_Scroll.cxx\n"
             "  Fl_Scrollbar.cxx\n"
             "  Fl_Shared_Image.cxx\n"
             "  Fl_Simple_Terminal.cxx\n"
             "  Fl_Single_Window.cxx\n"
             "  Fl_Slider.cxx\n"
             "  Fl_Spinner.cxx\n"
             "  Fl_Sys_Menu_Bar.cxx\n"
             "  Fl_System_Driver.cxx\n"
             "  Fl_Table.cxx\n"
             "  Fl_Table_Row.cxx\n"
             "  Fl_Tabs.cxx\n"
             "  Fl_Text_Buffer.cxx\n"
             "  Fl_Text_Display.cxx\n"
             "  Fl_Text_Editor.cxx\n"
             "  Fl_Tile.cxx\n"
             "  Fl_Tiled_Image.cxx\n"
             "  Fl_Tooltip.cxx\n"
             "  Fl_Tree.cxx\n"
             "  Fl_Tree_Item_Array.cxx\n"
             "  Fl_Tree_Item.cxx\n"
             "  Fl_Tree_Prefs.cxx\n"
             "  Fl_Valuator.cxx\n"
             "  Fl_Value_Input.cxx\n"
             "  Fl_Value_Output.cxx\n"
             "  Fl_Value_Slider.cxx\n"
             "  Fl_Widget.cxx\n"
             "  Fl_Widget_Surface.cxx\n"
             "  Fl_Window.cxx\n"
             "  Fl_Window_Driver.cxx\n"
             "  Fl_Window_fullscreen.cxx\n"
             "  Fl_Window_hotspot.cxx\n"
             "  Fl_Window_iconize.cxx\n"
             "  Fl_Wizard.cxx\n"
             "  Fl_XBM_Image.cxx\n"
             "  Fl_XPM_Image.cxx\n"
             "  Fl_abort.cxx\n"
             "  Fl_add_idle.cxx\n"
             "  Fl_arg.cxx\n"
             "  Fl_compose.cxx\n"
             "  Fl_display.cxx\n"
             "  Fl_get_system_colors.cxx\n"
             "  Fl_grab.cxx\n"
             "  Fl_lock.cxx\n"
             "  Fl_own_colormap.cxx\n"
             "  Fl_visual.cxx\n"
             "  filename_absolute.cxx\n"
             "  filename_expand.cxx\n"
             "  filename_ext.cxx\n"
             "  filename_isdir.cxx\n"
             "  filename_list.cxx\n"
             "  filename_match.cxx\n"
             "  filename_setext.cxx\n"
             "  fl_arc.cxx\n"
             "  fl_ask.cxx\n"
             "  fl_boxtype.cxx\n"
             "  fl_color.cxx\n"
             "  fl_cursor.cxx\n"
             "  fl_curve.cxx\n"
             "  fl_diamond_box.cxx\n"
             "  fl_draw.cxx\n"
             "  fl_draw_pixmap.cxx\n"
             "  fl_engraved_label.cxx\n"
             "  fl_file_dir.cxx\n"
             "  fl_font.cxx\n"
             "  fl_gleam.cxx\n"
             "  fl_gtk.cxx\n"
             "  fl_labeltype.cxx\n"
             "  fl_open_uri.cxx\n"
             "  fl_oval_box.cxx\n"
             "  fl_overlay.cxx\n"
             "  fl_overlay_visual.cxx\n"
             "  fl_plastic.cxx\n"
             "  fl_read_image.cxx\n"
             "  fl_rect.cxx\n"
             "  fl_round_box.cxx\n"
             "  fl_rounded_box.cxx\n"
             "  fl_set_font.cxx\n"
             "  fl_scroll_area.cxx\n"
             "  fl_shadow_box.cxx\n"
             "  fl_shortcut.cxx\n"
             "  fl_show_colormap.cxx\n"
             "  fl_symbols.cxx\n"
             "  fl_vertex.cxx\n"
             "  screen_xywh.cxx\n"
             "  fl_utf8.cxx\n"
             "  fl_encoding_latin1.cxx\n"
             "  fl_encoding_mac_roman.cxx\n"
             ")\n"
             "\n"
             "set (DRIVER_FILES\n"
             "  drivers/Android/Fl_Android_Application.cxx\n"
             "  drivers/Android/Fl_Android_System_Driver.cxx\n"
             "  drivers/Android/Fl_Android_Screen_Driver.cxx\n"
             "  drivers/Android/Fl_Android_Screen_Keyboard.cxx\n"
             "  drivers/Android/Fl_Android_Window_Driver.cxx\n"
             "  drivers/Android/Fl_Android_Image_Surface_Driver.cxx\n"
             "  drivers/Android/Fl_Android_Graphics_Driver.cxx\n"
             "  drivers/Android/Fl_Android_Graphics_Clipping.cxx\n"
             "  drivers/Android/Fl_Android_Graphics_Font.cxx\n"
             ")\n"
             "\n"
             "#source_group(\"Header Files\" FILES ${HEADER_FILES})\n"
             "source_group(\"Driver Source Files\" FILES ${DRIVER_FILES})\n"
             "#source_group(\"Driver Header Files\" FILES ${DRIVER_HEADER_FILES})\n"
             "\n"
             "set (CFILES\n"
             "  flstring.c\n"
             "  numericsort.c\n"
             "  vsnprintf.c\n"
             "  xutf8/is_right2left.c\n"
             "  xutf8/is_spacing.c\n"
             "  xutf8/case.c\n"
             ")\n"
             "\n"
             "set (CPPFILES\n"
             "  ${CPPFILES}\n"
             "  ${DRIVER_FILES}\n"
             ")\n"
             "\n"
             "set (SHARED_FILES ${CPPFILES} ${CFILES})\n"
             "set (STATIC_FILES ${SHARED_FILES})\n"
             "\n"
             "add_definitions(-DFL_LIBRARY)\n"
             "\n"
             "list_transform_prepend(STATIC_FILES \"${FLTK_DIR}/src/\")\n"
             "\n"
             "# now build app's shared lib\n"
             "add_library( fltk STATIC\n"
             "  ${STATIC_FILES}\n"
             ")\n"
             "\n"
             "set_target_properties( fltk\n"
             "    PROPERTIES\n"
             "    CLEAN_DIRECT_OUTPUT TRUE\n"
             "    COMPILE_DEFINITIONS \"FL_LIBRARY\"\n"
             ")\n"
             "\n"
             "target_include_directories(\n"
             "    fltk SYSTEM PRIVATE\n"
             // The path below is a terrible hack. The Android NDK include a file
             // name <version> somewhere, but instead of using the clang file,
             // if finds the FLTK "VERSION" file first. This path links directly to <version>
             // Alternative (cl/usr/include/cang only): -iwithsysroot /usr/include/c++/v1/
             "    ${CMAKE_SYSROOT}/usr/include/c++/v1/\n"
             "    ${FLTK_DIR}/\n"
             "    ${FLTK_DIR}/src/\n"
             "    ${FLTK_IDE_DIR}/\n"
             ")\n"
             "\n"
             "target_include_directories(\n"
             "    fltk PRIVATE\n"
             "    ${FLTK_DIR}/src/ )\n"
             );

  // create this for every application
  createFile(projectDir, "adjuster/build.gradle",
             "apply plugin: 'com.android.application'\n"
             "android {\n"
             "    compileSdkVersion 26\n"
             "    dependencies {\n"
             "        implementation project(':fltk')\n"
             "    }\n"
             "    defaultConfig {\n"
             "        applicationId 'org.fltk.adjuster'\n"
             "        minSdkVersion 14\n"
             "        targetSdkVersion 26\n"
             "        externalNativeBuild {\n"
             "            cmake {\n"
             "                arguments '-DANDROID_STL=c++_shared'\n"
             "            }\n"
             "        }\n"
             "    }\n"
             "    buildTypes {\n"
             "        release {\n"
             "            minifyEnabled false\n"
             "        }\n"
             "    }\n"
             "    externalNativeBuild {\n"
             "        cmake {\n"
             "            path 'src/main/cpp/CMakeLists.txt'\n"
             "        }\n"
             "    }\n"
             "}\n");
  createFile(projectDir, "adjuster/src/main/AndroidManifest.xml",
             "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
             "          package=\"org.fltk.adjuster\"\n"
             "          android:versionCode=\"1\"\n"
             "          android:versionName=\"1.0\">\n"
             "  <application\n"
             "      android:allowBackup=\"false\"\n"
             "      android:fullBackupContent=\"false\"\n"
             "      android:icon=\"@mipmap/ic_launcher\"\n"
             "      android:label=\"@string/app_name\"\n"
             "      android:hasCode=\"false\">\n"
             "    <activity android:name=\"android.app.NativeActivity\"\n"
             "              android:label=\"@string/app_name\">\n"
             "      <meta-data android:name=\"android.app.lib_name\"\n"
             "                 android:value=\"adjuster\" />\n"
             "      <intent-filter>\n"
             "        <action android:name=\"android.intent.action.MAIN\" />\n"
             "        <category android:name=\"android.intent.category.LAUNCHER\" />\n"
             "      </intent-filter>\n"
             "    </activity>\n"
             "  </application>\n"
             "</manifest>\n");
  createFile(projectDir, "adjuster/src/main/cpp/CMakeLists.txt",
             "cmake_minimum_required(VERSION 3.4.1)\n"
             "set(FLTK_DIR \"/Users/matt/dev/fltk-1.4.git\")\n" // FIXME: no absolute paths, obviously!
             "set(FLTK_IDE_DIR \"../../../..\")\n"
             "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=c++11\")\n"
             "add_library(\n"
             "    adjuster SHARED\n"
             "    \"${FLTK_DIR}/test/adjuster.cxx\"\n"
             ")\n"
             "target_include_directories(\n"
             "    adjuster SYSTEM PRIVATE\n"
             "    ${FLTK_DIR}/\n"
             "    ${FLTK_IDE_DIR}/\n"
             ")\n"
             // FIXME: is that for apps or only for the main FLTK library?
             //"set(CMAKE_SHARED_LINKER_FLAGS\n"
             //"    \"${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate\")\n"
             "target_link_libraries(\n"
             "    adjuster\n"
             // FIXME: there must be a better way to do this below
             //"    $<TARGET_FILE:fltk>\n"
             "    \"${FLTK_DIR}/build/AndroidStudio/fltk/.cxx/cmake/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libfltk.a\"\n"
             "    android\n"
             "    log\n"
             "    m\n"
             ")\n");
  createFile(projectDir, "adjuster/src/main/res/mipmap-mdpi/ic_launcher.png",
             mdpi_ic_launcher, sizeof(mdpi_ic_launcher));
  createFile(projectDir, "adjuster/src/main/res/mipmap-hdpi/ic_launcher.png",
             hdpi_ic_launcher, sizeof(hdpi_ic_launcher));
  createFile(projectDir, "adjuster/src/main/res/mipmap-xhdpi/ic_launcher.png",
             xhdpi_ic_launcher, sizeof(xhdpi_ic_launcher));
  createFile(projectDir, "adjuster/src/main/res/mipmap-xxhdpi/ic_launcher.png",
             xxhdpi_ic_launcher, sizeof(xxhdpi_ic_launcher));
  createFile(projectDir, "adjuster/src/main/res/values/strings.xml",
             "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<resources>\n"
             "    <string name=\"app_name\">adjuster</string>\n"
             "</resources>\n");
  // we may add fonts here: adjuster/src/main/assets/fonts/Roboto-Regular.ttf
}


int main(int argc, char **argv) {
  fl_message_title_default("Flink");
  Fl_Window *window = createMainWindow();

  char pathToFLTK[FL_PATH_MAX];
  strcpy(pathToFLTK, __FILE__);
  char *name = (char*)fl_filename_name(pathToFLTK);
  if (name && name>pathToFLTK) name[-1] = 0;
  name = (char*)fl_filename_name(pathToFLTK);
  if (name && name>pathToFLTK) name[-1] = 0;
  wSourceFolder->value(pathToFLTK);

  wProjectFolder->value("build/AndroidStudio");

  wDeleteExistingProject->value(1);

  window->show(argc, argv);
  return Fl::run();
}

//
// End of "$Id$".
//

