.PHONY: clean All

All:
	@echo "----------Building project:[ ce - Release ]----------"
	@"$(MAKE)" -f  "ce.mk"
clean:
	@echo "----------Cleaning project:[ ce - Release ]----------"
	@"$(MAKE)" -f  "ce.mk" clean
