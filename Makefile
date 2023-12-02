.PHONY: clean All

All:
	@echo "----------Building project:[ fft - Release ]----------"
	@"$(MAKE)" -f  "fft.mk"
clean:
	@echo "----------Cleaning project:[ fft - Release ]----------"
	@"$(MAKE)" -f  "fft.mk" clean
