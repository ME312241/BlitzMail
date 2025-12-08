# Visual Studio Configuration for Assimp

## For Windows Developers

To build this project in Visual Studio with Assimp support, you need to configure the project settings. This file provides instructions since the .vcxproj file cannot be easily modified in a cross-platform way.

### Option 1: Using vcpkg (Recommended)

1. Install vcpkg if you haven't already:
```cmd
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

2. Install Assimp:
```cmd
.\vcpkg install assimp:x86-windows
```

3. Open the project in Visual Studio - vcpkg integration will automatically configure include paths and libraries.

### Option 2: Manual Configuration

1. Download Assimp 5.x from: https://github.com/assimp/assimp/releases
2. Extract to a location (e.g., `C:\Libraries\Assimp`)

3. In Visual Studio:
   - Right-click project → Properties
   - **Configuration**: All Configurations
   - **Platform**: Win32

4. Add Include Directory:
   - C/C++ → General → Additional Include Directories
   - Add: `C:\Libraries\Assimp\include`

5. Add Library Directory:
   - Linker → General → Additional Library Directories
   - Add: `C:\Libraries\Assimp\lib`

6. Add Library Dependency:
   - Linker → Input → Additional Dependencies
   - Add: `assimp-vc143-mt.lib` (or appropriate version for your VS)

7. Copy DLL to output directory:
   - Copy `assimp-vc143-mt.dll` to your Debug and Release folders
   - Or add a Post-Build Event:
     ```
     copy "C:\Libraries\Assimp\bin\assimp-vc143-mt.dll" "$(OutDir)"
     ```

### Required Changes to OpenGL3DTemplate.vcxproj

Add to Debug configuration `<ItemDefinitionGroup>`:
```xml
<AdditionalIncludeDirectories>
  $(OutputPath)\..;
  C:\Libraries\Assimp\include;
  %(AdditionalIncludeDirectories)
</AdditionalIncludeDirectories>

<AdditionalLibraryDirectories>
  $(OutputPath)\..;
  C:\Libraries\Assimp\lib;
  %(AdditionalLibraryDirectories)
</AdditionalLibraryDirectories>

<AdditionalDependencies>
  glut32.lib;
  assimp-vc143-mt.lib;
  %(AdditionalDependencies)
</AdditionalDependencies>
```

### Troubleshooting

**"Cannot open include file: 'assimp/Importer.hpp'"**
- Check that include directories are configured correctly
- Verify Assimp is installed in the specified location

**"Unresolved external symbol" errors**
- Check that library directories are configured correctly
- Verify library filename matches your Visual Studio version (vc142, vc143, etc.)

**"Cannot find assimp-*.dll"**
- Copy the DLL to your Debug/Release folder
- Or add the Assimp bin directory to your PATH

See ASSIMP_INTEGRATION.md for more details.
