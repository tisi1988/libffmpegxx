# Change Log
All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.0.6-alpha] - XXXX-XX-XX
 
### Added

### Changed

### Fixed

## [0.0.5-alpha] - 2021-12-10
 
### Added
- Decoder
- Decoding sample app

### Changed

### Fixed
- Wrong warning message when closing demuxer

## [0.0.4-alpha] - 2021-12-05
 
### Added
- Implementation of AVFrame

### Changed

### Fixed
- Errors inside AVPacketImpl now logs the error before throwing
- Check error on IAVPacket::redToPacket()
- Fixed AVPacket::getRawData() for non-referenced packets

## [0.0.3-alpha] - 2021-12-04
 
### Added
- Implemented muxing
- Make Logger, Demuxer, Muxer thread-safe
- Implemented more logging

### Changed
- Set loglevel in Demuxer example app
- Replaced example app for Demuxer with a remuxing (Demuxer/Muxer) example
- Remuxing app does not print IAVPacket info always. This can be enabled through log level DEBUG

### Fixed
- FFmpeg logging message parsing
- Fix AVPacket include for FFmpeg versions older than 4.3


## [0.0.2-alpha] - 2021-12-01
 
### Added
- Implement Demuxer
- Example app for Demuxer

### Changed
- Moved to C++17

### Fixed
- Some bugfixes
 
## [0.0.1-alpha] - 2021-11-28
 
### Added
- avcodec::IAVPacket and implementation
- utils::Logger
- time::Timestamp
- time::Timebase
- utils::Rational
- Example apps for: Version, Logger, AVPacket, Timestamp.

