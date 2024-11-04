.PHONY: clean All

All:
	@echo "----------Building project:[ fd - Release ]----------"
	@"$(MAKE)" -f  "fd.mk"
clean:
	@echo "----------Cleaning project:[ fd - Release ]----------"
	@"$(MAKE)" -f  "fd.mk" clean
