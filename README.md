# Lazyploy Launcher

Lazyploy Launcher is the 'engineer' component of [Lazyploy](https://github.com/Allar/lazyploy-server). Instead of creating and maintaining a set of batch files per project, Lazyploy Launcher aims to consolidate batch operations such as launching clients, cooking, and deploying of builds. It runs as a standalone program built against your UE4 Engine source and runs on Slate. It also contains a Session Frontend tab for easy remote-monitoring of other UE4 instances. 

## Installation

### With UE4 Built From Source Code

1. Clone this repo and then copy the contents of the Engine folder into your UE4's Engine folder.
1. Regenerate Engine project files.
1. Build `LazyployLauncher` with the `Development` configuration.
1. Run `Engine\Binaries\Win64\LazyployLauncher.exe`.

### With Binary/Launcher Version of UE4

    @TODO

Developed on 4.10.

I offer no support currently.

Forum thread: https://forums.unrealengine.com/showthread.php?99467