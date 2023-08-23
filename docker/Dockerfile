FROM debian:trixie-20230814

RUN apt-get clean
RUN apt-get update
RUN apt-get install -yy --no-install-recommends bash less build-essential gdb

ARG LNFS_UID=1000

RUN useradd -c lnfs -s /bin/bash -u "${LNFS_UID}" -U -m lnfs
RUN id -a lnfs

USER lnfs:lnfs

ENV USER lnfs
ENV HOME /home/lnfs

RUN install -v -d -m 0750 ${HOME}/src

WORKDIR ${HOME}/src
CMD exec /bin/bash -il
