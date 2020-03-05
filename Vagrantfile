Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.provision "shell", path: "scripts/apt.sh"
  config.vm.provision "shell", path: "scripts/pin.sh"
  
  config.vm.provision "shell", :run => 'always', inline: <<-SHELL
    echo 'source /vagrant/scripts/env.sh' > /etc/profile.d/pin-env.sh
  SHELL

  config.ssh.extra_args = ["-t", "cd /vagrant; bash --login"]
end
