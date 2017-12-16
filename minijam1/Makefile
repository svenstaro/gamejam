.PHONY: default
default: node_modules build

.PHONY: node_modules
node_modules:
	npm install truffle

.PHONY: build
build:
	node_modules/.bin/truffle compile

.PHONY: test
test:
	node_modules/.bin/truffle test
