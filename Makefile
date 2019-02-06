.PHONY: clean All

All:
	@echo "----------Building project:[ DDEC2 - Release ]----------"
	@$(MAKE) -f  "DDEC2.mk"
clean:
	@echo "----------Cleaning project:[ DDEC2 - Release ]----------"
	@$(MAKE) -f  "DDEC2.mk" clean
