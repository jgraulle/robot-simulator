FROM ubuntu:24.10

ARG USER_NAME=user
ARG USER_ID=1000
ARG GROUP_ID=1000

RUN apt update
RUN apt upgrade --yes
RUN apt install --yes g++ gdb make cmake libjsoncpp-dev libsfml-dev libasio-dev
RUN if getent passwd $USER_ID ; then userdel --remove $(id --name --user $USER_ID); fi
RUN groupadd --gid $GROUP_ID $USER_NAME
RUN useradd --create-home --shell /bin/bash --uid $USER_ID --gid $GROUP_ID $USER_NAME
USER $USER_NAME
CMD /bin/bash
