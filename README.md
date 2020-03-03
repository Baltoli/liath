# Liath

Dynamic binary instrumentation tools written using [Pin][pin]. The intention for
these tools is to instrument dynamic behaviour of programs so that formal models
can be derived from them (for example, via program synthesis).

## Getting Started

A [vagrant][Vagrant] environment is supplied to make development and evaluation
of these tools easier.

**Prerequisites**:
* [Vagrant][vagrant]
* [VirtualBox][vbox]

With the prerequisites installed, start up a virtual machine. This will take
some time the first time you run it:
```shell
vagrant up
vagrant ssh
```

You should find yourself back at the project root, but inside an Ubuntu VM. To
build all the Pin tools and demo programs, run:
```shell
make
```

To run a pin tool:
```shell
pin -t $path_to_tool_shared_lib -- $some $command $line
```

[pin]: https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool
[vagrant]: https://www.vagrantup.com/
[vbox]: https://www.virtualbox.org/
