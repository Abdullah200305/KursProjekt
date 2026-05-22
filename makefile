all:
	$(MAKE) -C client
	$(MAKE) -C server

clean:
	$(MAKE) -C client clean
	$(MAKE) -C server clean

run:
	start cmd /k "cd server && server.exe"

	timeout /t 2 >nul

	start cmd /k "cd client && client.exe"
	start cmd /k "cd client && client.exe"