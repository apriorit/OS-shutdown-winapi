# Handling the OS shutdown event using WinAPI 

## About

This project is a collection of three utilities illustrating detection and handling the OS shutdown events using WinAPI for console applications, GUI applications, and services.

We need to detect computer shutdown event to correctly finish the work of our applications, in particular:

- Save all needed data,
- If we have a network distributed system, we should notify all its components that this current component will be shut off,
- Update internal event log adding record about the component shutdown.

## Implementation

C++ (Visual Studio) is used.

**Console applications:** The implementation is based on the corresponding WinAPI events handling.
**GUI applications:** The implementation is based on handling WM_QUERYENDSESSION and WM_POWERBROADCAST window messages.
**Services:** Implemented mechanism is pretty much the same as the one used for console applications, but has its specifics in creating and registering callback functions and Windows event handling.

For more detailed tutorial and example explanations, please read the [related article](https://www.apriorit.com/dev-blog/413-win-api-shutdown-events).

## License

Licensed under the MIT license. © Apriorit.
