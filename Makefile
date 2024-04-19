.PHONY: clean All

All:
	@echo "----------Building project:[ patest - Release ]----------"
	@"$(MAKE)" -f  "patest.mk"
clean:
	@echo "----------Cleaning project:[ patest - Release ]----------"
	@"$(MAKE)" -f  "patest.mk" clean
