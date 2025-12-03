# BlitzMail - Quick Start Guide

## What is BlitzMail?
BlitzMail is a 3D rural mail delivery game where you play as a mailman collecting packages in a scenic countryside environment.

## Controls
| Key/Input | Action |
|-----------|--------|
| W | Move Forward |
| A | Move Left |
| S | Move Backward |
| D | Move Right |
| Space | Jump |
| C | Crouch/Stand |
| V | Toggle Camera View |
| Mouse | Look Around |
| ESC | Exit Game |

## Game Objective
Collect all 5 packages scattered around the rural scene!

## Camera Modes
- **First-Person** (Default): See through the mailman's eyes
- **Third-Person**: See your character from behind
- Press **V** to toggle between modes

## Scene Features

### Environment
- Large grass terrain (200x200 units)
- 4 rural houses (farmhouses and cottages)
- 8 trees providing shade
- Wooden fences
- Rock formations
- Wheat and carrot crop fields
- Minecraft-style grass blocks

### Lighting
- **Sun**: Rotates around the scene with dynamic color changes
  - Bright yellow during daytime
  - Orange during sunset/sunrise
  - Dim blue at night
- **Street Lamps**: 4 lamps that automatically light up at night with flickering effect

### Collectibles
- **5 Packages** to collect
- Packages rotate for easy spotting
- Walk close to a package to collect it automatically
- Progress is shown in the console

## Tips
- Explore the entire scene to find all packages
- Use third-person view (V key) to see your character
- Packages are located at:
  - Near the eastern house
  - By the western fence
  - South field area
  - Northern cottage
  - Central-eastern rocks
- Watch the beautiful day/night cycle as you play!
- Crouching (C) makes you move slower but might help in tight spots

## Technical Requirements
- Windows with Visual Studio
- OpenGL and GLUT libraries
- Minimum 800x600 display

## Building
1. Open `OpenGL3DTemplate.sln` in Visual Studio
2. Build the project (F7)
3. Run (F5)

## Troubleshooting
- **Black screen?** Check that OpenGL drivers are installed
- **Can't move?** Try clicking in the window to capture mouse
- **Lag?** Try updating graphics drivers

## Have Fun!
Enjoy exploring the rural countryside and collecting all the packages!

---
For detailed implementation notes, see IMPLEMENTATION_NOTES.md
For requirements verification, see VERIFICATION_CHECKLIST.md
