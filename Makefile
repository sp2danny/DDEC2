.PHONY: clean All

All:
	@echo "----------Building project:[ player - Debug ]----------"
	@"$(MAKE)" -f  "player.mk"
clean:
	@echo "----------Cleaning project:[ player - Debug ]----------"
	@"$(MAKE)" -f  "player.mk" clean
