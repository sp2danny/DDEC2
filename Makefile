.PHONY: clean All

All:
	@echo "----------Building project:[ de - Release ]----------"
	@"$(MAKE)" -f  "de.mk"
clean:
	@echo "----------Cleaning project:[ de - Release ]----------"
	@"$(MAKE)" -f  "de.mk" clean
