# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Introduced `IDisplay` interface to decouple display logic from hardware-specific libraries.
- Implemented `TftDisplay` driver for standard `TFT_eSPI` based screens.
- Added support for T-Display S3 Amoled with a new `AmoledDisplay` driver.
- Added `basicUsageAmoled` example.

### Changed
- Refactored `UptDisplay` to use the `IDisplay` interface.
- Improved rendering performance on buffered displays with `flush()` method.

## [0.4.1]

### Fixed

- Values randomly not displayed with less than 3 digits

### Added

## [0.4.0]

### Added

- Display of H2 and pressure signals

## [0.3.0]

### Changed

- Moved to stable UPT Core 1.0.0

## [0.2.2]

### Fixed

- Display of single digit values

## [0.2.1]

### Changed

- Fix display of sensor name
- Change display of sensor values to fixed notation

## [0.2.0]

### Changed

- Update to build with arduino-upt-core >= 0.9.0
- Added namespace
- Use std libraries

## [0.1.0] - 2025-06-05

### Added

- Initial release of the library
