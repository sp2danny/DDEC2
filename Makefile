.PHONY: clean All

All:
	@echo "----------Building project:[ fd - Debug ]----------"
	@"$(MAKE)" -f  "fd.mk"
clean:
	@echo "----------Cleaning project:[ fd - Debug ]----------"
	@"$(MAKE)" -f  "fd.mk" clean
