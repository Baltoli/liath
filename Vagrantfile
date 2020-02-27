PIN_URL = "https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.11-97998-g7ecce2dac-gcc-linux.tar.gz"

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y gcc-8 g++-8

    curl -OL #{PIN_URL}
    mkdir pin
    tar xvf *.tar.gz -C pin --strip-components 1
    rm *.tar.gz
  SHELL
end
