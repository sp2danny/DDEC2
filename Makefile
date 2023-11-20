.PHONY: clean All

All:
	@echo "----------Building project:[ ce - Debug ]----------"
	@"$(MAKE)" -f  "ce.mk"
clean:
	@echo "----------Cleaning project:[ ce - Debug ]----------"
	@"$(MAKE)" -f  "ce.mk" clean
