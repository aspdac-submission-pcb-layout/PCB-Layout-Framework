git submodule update --init --recursive
docker build -t pcbprapp \
  --no-cache \
  --build-arg USER_ID=$(id -u) \
  --build-arg GROUP_ID=$(id -g) .
docker run -u $(id -u ${USER}):$(id -g ${USER}) -v $(pwd):/PCB-Layout-Framework pcbprapp bash -c "./PCB-Layout-Framework/jenkins/install.sh"
