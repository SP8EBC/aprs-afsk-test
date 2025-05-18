OBJS := main.o correlation.o
DEFINES := -DSAMPLERATE=9600 -DBITRATE=1200
INCLUDES := -I/usr/include -I.
LIBS := -lconfig -lconfig++
LIBS_DIRS := -L/lib/x86_64-linux-gnu -L/lib/i386-linux-gnu -L/usr/lib
WARNS := -Wmaybe-uninitialized -Wpedantic -Wno-return-local-addr -Wparentheses


all: aprs-afsk-test

clean:
	rm -f -- *.o *.lst *.d *.svg aprs-afsk-test

aprs-afsk-test: $(OBJS)
	@echo 'linking $@'
	g++ $(LIBS) $(LIBS_DIRS) -o $(@) $(OBJS)
	objdump --source --all-headers --demangle --line-numbers --wide "$@" > "$@.lst"

%.o : %.cpp
	@echo 'Building: $<'
	g++ $(DEFINES) $(INCLUDES) -march=skylake -std=c++17 -g3 -O3 $(WARNS) -c -fmessage-length=0 -MMD -MP -MF"$(patsubst %.o,%.d,$(@))" -MT"$(@)" -o "$(@)" "$(<)"