FROM ubuntu:latest
MAINTAINER Isaiah Solomon "isaiah.c.solomon@gmail.com"
RUN apt-get update -y
RUN apt-get install -y python-pip python-dev build-essential python-tk python-matplotlib
RUN pip install --upgrade pip
RUN pip install -U tensorflow sklearn scipy numpy
COPY . /app
WORKDIR /app
ENTRYPOINT ["python"]
CMD ["a4q2.py"]
