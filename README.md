# Config Utility

A simple C++ config file utility, which can also parse command line arguments.

# Docs

The config files must be ordered as `key=value` pairs on each line, as such:

```
server.name = Tortuga
server.address = 127.0.0.1
server.port = 21795
server.dbname = database.db
```

Keys beginning with `_config` are reserved for internal use.

You can pass multiple `--config=[filename]` into it via the command line to specify multiple config files to load, or pass in a number of key-value pairs as command line arguments (or both):

```
app.exe -server.name=Tortuga -server.address=127.0.0.1 -server.port=21795 -server.dbname=database.db
```

# Example

```
#include "config_utility.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
	ConfigUtility config;

	//config file to load, skip missing files (default = flase), argc (default = 0), argv (default = nullptr)
	config.LoadConfig("rsc/config.cfg", true, argc, argv);

	//will print the value for the given key
	std::cout << config["key"] <<std::endl;

	return 0;
}
```
