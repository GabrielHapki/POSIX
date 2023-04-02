FROM mycpp

LABEL maintainer="Gabriel Augusto Costa" \
      description="POSIX" \
      version="0.0.1"

WORKDIR /POSIX

COPY CMakeLists.txt /POSIX/
COPY examples /POSIX/examples
COPY libs /POSIX/libs
COPY tests /POSIX/tests

RUN mkdir /POSIX/build
RUN cmake -B/POSIX/build -S/POSIX
RUN cd /POSIX/build && make
