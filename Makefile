.PHONY: clean All

All:
	@echo "----------Building project:[ player - Release ]----------"
	@$(MAKE) -f  "player.mk"
clean:
	@echo "----------Cleaning project:[ player - Release ]----------"
	@$(MAKE) -f  "player.mk" clean
