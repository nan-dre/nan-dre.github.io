FROM ruby:2.7-slim
RUN apt-get update && apt-get install make gcc g++ bash cmake git -y
RUN gem install bundler jekyll
WORKDIR /site
