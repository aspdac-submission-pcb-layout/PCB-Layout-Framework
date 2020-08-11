docker run -u $(id -u ${USER}):$(id -g ${USER}) -v $(pwd):/PCB-Layout-Framework pcbprapp bash -c "cd PCB-Layout-Framework;./ubuntu_install.sh; ./tests/regression-py.sh"
