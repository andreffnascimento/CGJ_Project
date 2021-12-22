<h1 style="color: #4682B4; font-size: 40px;" align="center">Micro Machines 3D Engine Reference</h1>

<i>This file contains information regarding the functionality of the engine used for the Micro Machines 3D project. There are also some usage examples spread across the document.</i>

<h2 align="center"> Engine Modules </h2>

<p align="center">
  <a href="#application-module">Application</a> •
  <a href="#math-module">Math</a> •
  <a href="#renderer-module">Renderer</a> •
  <a href="#scene-module">Scene</a> •
</p>

<br>
<br>
<br>

# Application Module

<p align="center">
  <a href="#app">App</a> •
  <a href="#callback">Callback</a> •
  <a href="#input-handler">Input Handler</a>
</p>

The *Application* contains the primary functionality of the engine. It works as the entry point for the application and controls the flow.

<br>

## App
The *App* is a singleton class. It is also the entry point of the engine. The *App* class initializes the window and sets the callback functions for *GLUT*. All of this functions are stored in the callback file. Most of them are friends of the *App* class, so that they can use its attributes freely.

Furthermore, the *App* contains a pointer to the current *Scene* and an *Input Handler* and Renderer instance. This attributes can be accessed through static methods in the class.

***@See:*** *MicroMachines3D#main()* to understand how the engine is initialized.

<br>

## Callback
The callback file contains global functions to process the callback events from *GLUT*. The methods and there usage are described in the table below:

<table align="center">
    <thead>
        <tr>
            <th><div style="font-size: 18px" align="center">Method</div></th>
            <th><div style="font-size: 18px" align="center">Usage</div></th>
        </tr>
    </thead>
    <tbody>
        <tr> <td><i>terminateApp</i></td> <td>Gracefully terminates the engine when the user closes the game</td> </tr>
        <tr> <td><i>timer</i></td> <td>Displays the FPS on the window</td> </tr>
        <tr> <td><i>refresh</i></td> <td>Refreshes the screen (every 1/60 seconds)</td> </tr>
        <tr> <td><i>viewportResize</i></td> <td>Resizes the window by changing the active camera settings (stored in the current <i>Scene</i>)</td> </tr>
        <tr> <td><i>displayScene</i></td> <td>Updates the <i>Scene</i> and clears the input every frame</td> </tr>
        <tr> <td><i>processKeys</i></td> <td>Stores the press of a key in the <i>Input Handler</i></td> </tr>
        <tr> <td><i>processMouseButtons</i></td> <td>Stores the click of a mouse button on the <i>Input Handler</i></td> </tr>
        <tr> <td><i>processMouseMotion</i></td> <td>Stores the current position of the mouse on the <i>Input Handler</i></td> </tr>
        <tr> <td><i>mouseWheel</i></td> <td>Stores the direction of the mouse wheel on the <i>Input Handler</i></td> </tr>
    </tbody>
</table>

<br>

## Input Handler
The *Input Handler* is responsable for storing all the input events so that they can be retrived by the *Scripts*. This class only acts as a buffer for input events, so that we don't need to rely on *GLUT*.

<br>
<br>
<br>

# Math Module

<p align="center">
  <a href="#transform">Transform</a>
</p>

The *Math* module contains math functions to perform certain operations over the engine components. The *AVTmathLib* is also stored within this module.

<br>

## Transform
The *Transform* class contains a set of functions to apply transforms to the entities of the scene. These transforms are:
- Translation
- Rotation
- Scale

This class is a bit complex but the available methods are the following:
```
static void translate(const Entity& entity, const Coords3f& translation, bool applyToGroup = true);
static void rotate(const Entity& entity, const Coords3f& rotation, bool applyToGroup = true);
static void scale(const Entity& entity, const Coords3f& scale, bool applyToGroup = true);

static void translateTo(const Entity& entity, const Coords3f& newTranslation, bool applyToGroup = true);
static void rotateTo(const Entity& entity, const Coords3f& newRotation, bool applyToGroup = true);
static void scaleTo(const Entity& entity, const Coords3f& newScale, bool applyToGroup = true);
```

The first thing to realise is that there are two types of transformations. The first type of transformations apply a translation/rotation/scale to an entity's transform. The second sets the value of the transform to the one specified.

Furthermore notice the *applyToGroup* flag. By default, applying a transform to an entity also applies it to all transform components of all the sub-entities of the first one. This flag can be set to false to prevent this, but this usually shouldn't be done. This feature is very usefull, for instance to apply a transformation to an object that is made of multiple objects.

***@see:*** *MicroMachines3D/world/car.h* for an example. The car is made of multiple objects. The feature allows to translate/rotate/scale the entire car with a single call.

<br>
<br>
<br>

# Renderer Module

<p align="center">
  <a href="#renderer">Renderer</a>
</p>

The *Renderer* module is responsable for drawing things on to the screen. It is responsable for loading the shaders and the vertex information on to the GPU, making sure the camera and lights are properly set, etc...

<br>

## Renderer
The renderer class only contains information regarding both the graphics and text shaders. It is also responsable for loading *GLEW* during the engine's initialization. Finally, the renderer has the responsability of using the *AVTmathLib* to ensure things are rendered in a proper way. This is done via the methods:

<table align="center">
    <thead>
        <tr>
            <th><div style="font-size: 18px" align="center">Method</div></th>
            <th><div style="font-size: 18px" align="center">Usage</div></th>
        </tr>
    </thead>
    <tbody>
        <tr> <td><i>updateViewport</i></td> <td>Updates the viewport using the *CameraComponent* of the active camera (@see: auxiliary functions)</td> </tr>
        <tr> <td><i>initSceneRendering</i></td> <td>Initializes the rendering of the scene, by clearing the color and depth buffer</td> </tr>
        <tr> <td><i>renderCamera</i></td> <td>Renders the camera using its position and target vector</td> </tr>
        <tr> <td><i>renderObjects</i></td> <td>Renders all the meshes of the scene according to their transfom component (@see: auxiliary functions)</td> </tr>
    </tbody>
</table>

<br>
<br>
<br>

# Scene Module

<p align="center">
  <a href="#components">Components</a> •
  <a href="#ecs-registry">ECS Registry</a> •
  <a href="#entity">Entity</a> •
  <a href="#scene">Scene</a> •
  <a href="#script">Script</a>
</p>

<br>

## Components
*Components* are the **everything** in the engine. They store the data required to do all kinds of processing. There are multiple types of *Components*, and each entity can be associated with at most one from each type. Their types are explained below:

<table align="center">
    <thead>
        <tr>
            <th><div style="font-size: 18px" align="center">Component</div></th>
            <th><div style="font-size: 18px" align="center">Usage</div></th>
        </tr>
    </thead>
    <tbody>
        <tr> <td><i>TagComponent</i></td> <td>Stores the name of the entity. Each entity must have a tag (Entity$<'id'> if none is provided). Tags must be unique because they allow to find the entity on the scene (@see: Scene#getEntityByTag())</td> </tr>
        <tr> <td><i>TransformComponent</i></td> <td>Stores the position, rotation and scale of an entity. They can only be manipulated using the Transform class (@see: engine/math/transform.h)</td> </tr>
        <tr> <td><i>GroupComponent</i></td> <td>Groups multiple entities under another. These sub-entities must be created using the group's addNewEntity method to prevent cyclic groups (@see: GroupComponent#addNewEntity()). Transforms can be applied to groups (@see: engine/math/transform.h))</td> </tr>
        <tr> <td><i>CameraComponent</i></td> <td>Stores the camera settings (projection type, visualization volume, target position, etc...). Scene cameras are required to have a CameraComponent (@see: engine/scene/sceneCamera.h)</td> </tr>
        <tr> <td><i>MeshComponent</i></td> <td>Stores a pointer to the mesh data of an entity. All entities with meshes are retrieved and rendered at the end of the frame processing</td> </tr>
        <tr> <td><i>ScriptComponent</i></td> <td>Stores a pointer to a script. Scripts are instances that run at the beggining of each frame (@see: engine/scene/script.h). New scripts can be created by extending the script class (@see: MicroMachines3D/scripts/ for examples)</td> </tr>
      </td> </tr>
    </tbody>
</table>

**Note:** More components will be added in the near future. The lighting and some physics related components are some examples.

<br>

## ECS Registry
The *ECS Registry* is responsable for storing and managing all the components of the engine. Its a complicated class, but the most important thing to know is that each type of component is stored in a map, with the *entity's id* as key. It's done this way for efficiency reasons.

***@see:*** *ECSRegistry#getComponents()* to look at the way components are stored <br>
***@see:*** *ECSRegistry#EntityHandle()* to look at the *entity id*

<br>

## Entity
In the engine, the *Entity* is only used to stored an *id*. This *id* can be used to retrive a component from that entity. Entities do not get stored anywhere in the engine because they do not contain any important data for the system. They are just a way to associated components from the same object together.

<br>

## Scene
The *Scene* contains an *ECS Registry* with all the entities stored in it. This class can be extended in order to be personalized by the developer. Furthermore, the scene also contains the camera currently active.

***@see:*** *MicroMachines3D/scenes/MicroMachines3DScene.h*

Each frame, the *App* calls the update method of the scene (@see: Scene#onUpdate()). During the update a couple of things happend in the following order:
1. All the scripts run (@see: Script#onUpdate())
2. The physics components are processed
3. The active camera is set
4. The lighting components are rendered
5. The solid objects are rendered
6. The transparent objects are rendered
7. The special effects are processed

**Note:** Some of these steps are not yet implemented!

<br>

## Script
The *Script* is a abstract class to be extended by the developer. It has two main methods:
- *onCreate()* that runs when the scene is set on the app
- *onUpdate()* that runs each frame to do some sort of processing

***@see:*** *MicroMachines3D/scripts/* for some scrips examples