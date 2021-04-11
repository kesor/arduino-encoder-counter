# https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc
#
verify:
	mkdir -p $(CURDIR)/build
	arduino --verify \
	  --board arduino:avr:nano:cpu=atmega328 \
	  --verbose \
	  --pref build.path=$(CURDIR)/build \
	  arduino-encoder-counter.ino
	@echo

upload:
	mkdir -p $(CURDIR)/build
	arduino --upload \
	  --board arduino:avr:nano:cpu=atmega328 \
	  --verbose \
	  --useprogrammer \
	  --verbose-upload \
	  --pref build.path=$(CURDIR)/build \
	  --pref programmer=ATTinyCore:usbtinyisp \
	  --pref upload.verify=true \
	  arduino-encoder-counter.ino
	@echo

serial:
	@socat open:/dev/ttyUSB0,b115200,echo=0,raw -

.PHONY: verify
