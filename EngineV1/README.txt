--Arne Borremans 2DAE01 Programming 4 Engine

I have made an engine that you can adapt on screen ( you can place, delete, alter variables via an interface ).
Scenes can be created and deleted; you can add gameobjects to these scenes which you can move around in the hierarchy tree to make parents / childs
For each object in the scene, you can add as many components as you want and you can change the parameters of these components to your liking inside of the editor, so no need to manually go change it in the code.

You can add prefabs from gameobjects that you created in the scene by dragging them into the bottom window. 
You can then add these prefabs to any scene that you want.
The bottom window also shows all of your scripts and all of your beaviours (for the FSM)


-- GameLoop --
Everything start with the game loop, I have chosen for a fixed update time step with variable rendering.
Because of this, the game is never too fast or too slow and when your pc is running too slow, we first catch up with updating every missed frame before we start rendering again.


-- Finite State Machine --
One of the great features is the FSM, it obviously uses states, but it also uses the command pattern since you can change the transitions between the states inside of the editor, you
can set the transition by any of the behaviours you made

-- Input Manager --
The input manager is similiar to that from UE4, you can create an action with a unique identifier ("Jump", "Move") and you can add any key to this action, so also similar to a command pattern

-- Observer Pattern --
The observer pattern is used on the rigidbody, which is the subject, whenever this rigidbody collides, it will send out with who it has collided, for anyone that is interested in that information

-- Script Component --
With this component, you can add any custom made script to an object, this way it is easy to add custom behaviour to objects

-- Rigidbody Component + Box Collider --
Box2D is used for the physics side of the engine, both the rigidbody and the box collider work in tandem to create collision and overlapping behaviour with the world and other objects


-- Components --
There are a lot of different components:
- Tranform Component
- Finite State Machine Component
- Texture Component
- Text Component
- Movement Component
- Rigidbody Component
- Box Collider Component
- Script Component




https://github.com/ArneBorr/EngineV1




