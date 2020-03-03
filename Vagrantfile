PIN_URL = "https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.11-97998-g7ecce2dac-gcc-linux.tar.gz"

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.provision "shell", inline: <<-SHELL
    # Install necessary packages
    apt-get update
    apt-get install -y build-essential gcc-8 g++-8

    # Get the Pin binary distribution from Intel
    if [ ! -d "pin" ]; then
      echo "Downloading and unpacking pin..."
      curl -OL #{PIN_URL}
      mkdir pin
      tar xvf *.tar.gz -C pin --strip-components 1
      rm *.tar.gz
      chown -R vagrant:vagrant pin
    fi
  SHELL
  
  config.vm.provision "shell", :run => 'always', inline: <<-SHELL
    echo 'source /vagrant/scripts/env.sh' > /etc/profile.d/pin-env.sh
  SHELL

  config.ssh.extra_args = ["-t", "cd /vagrant; bash --login"]
end
