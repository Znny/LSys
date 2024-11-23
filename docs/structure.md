# Project Structure

This document outlines the organization of the project and the purpose of each directory and file. The structure is designed to prioritize scalability, maintainability, and modularity.

## Directory Layout
- **`include/`**: Contains header files for each module (e.g., LSystems, Rendering, UI, Utilities). These files define the interfaces and reusable components of the project.
- **`src/`**: Contains source files implementing the logic declared in `include/`. Files are grouped by module for clarity.
- **`resource/`**: Stores non-code assets like shaders and configuration files used for rendering and system configurations.
- **`lib/`**: Houses third-party libraries such as glad, GLFW, glm, and KHR for supporting rendering and math operations.
- **`docs/`**: Includes documentation, design notes, and task management files.

## Modules
### **LSystems**
Handles plant generation using L-systems. Includes components for defining and managing the L-system rules and alphabets.

### **Rendering**
Manages the rendering pipeline, including camera handling, shaders, and drawing basic shapes.

### **UI**
Handles user interactions using Dear ImGui. Provides sliders, buttons, and input fields to adjust plant parameters.

### **Utilities**
Provides general-purpose tools such as a `Transform` class for spatial operations, `Turtle` graphics for interpreting L-system output, `Logging` utilities, and the `DynamicSet` container.

This structure is designed to separate concerns while allowing easy addition of new features and modules.
