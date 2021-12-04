# Change Log
All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.0.3-alpha] - XXXX-XX-XX
 
### Fixed
- FFmpeg logging message parsing
- Set loglevel in Demuxer example app
- Fix AVPacket include for FFmpeg versions older than 4.3
- Implemented muxing
- Replaced example app for Demuxer with a remuxing (Demuxer/Muxer) example
- Make Logger, Demuxer, Muxer thread-safe


## [0.0.2-alpha] - 2021-12-01
 
### Added
- Moved to C++17
- Some bugfixes
- Implement Demuxer
- Example app for Demuxer

 
## [0.0.1-alpha] - 2021-11-28
 
### Added
- avcodec::IAVPacket and implementation
- utils::Logger
- time::Timestamp
- time::Timebase
- utils::Rational
- Example apps for: Version, Logger, AVPacket, Timestamp.

