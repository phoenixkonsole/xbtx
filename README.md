Cura Core integration/staging tree
=====================================

https://cura-blockchain.org

What is Cura Blockchain?
----------------

CURA (XBTX) is a decentralized, peer-to-peer digital currency that aims to provide a secure, fast, and low-cost method 
of transferring value worldwide. It is a fork of the Ravencoin project and uses the Scrypt^2 algorithm
to ensure a fair and efficient mining process. 
CURA also offers a platform for accessing AI models such as ChatGPT, as well as a marketplace for medical services, coaching, 
and other healthcare-related products. Also Cura will be the experimental PoC for a decentralized Hydrogen Supply Management Platform. 
This whitepaper aims to provide an overview of the CURA project, including its features, specifications, and use cases. 


License
-------

Cura Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/phoenixkonsole/xbtx/tags) are created
regularly to indicate new official, stable release versions of Cura Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

Testnet is now up and running and available to use during development. There is an issue when connecting to the testnet that requires the use of the -maxtipage parameter in order to connect to the test network initially. After the initial launch the -maxtipage parameter is not required.

Use this command to initially start curad on the testnet. <code>./curad -testnet -maxtipage=259200</code>

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`


### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.


About Cura
----------------
CURA (XBTX) is a unique digital currency that provides fast, secure, and low-cost transactions, 
while also offering tokenization capabilities and access to AI models such as ChatGPT. 
With its low transaction fees and fast block times, CURA has the potential to become a popular digital currency for peer-to-peer transactions,
tokenization, and healthcare-related services. 
The decentralization of the currency ensures that it is more resilient and resistant to censorship and government intervention, 
providing a secure and transparent platform for users to conduct their business.

In addition to its existing features, CURA (XBTX) will also offer a Hydrogen Availability Management Platform, 
providing a decentralized solution to manage the availability and distribution of hydrogen fuel. 
This platform will use smart contracts and blockchain technology to create a transparent and secure system for tracking the production, 
distribution, and consumption of hydrogen.

The Hydrogen Availability Management Platform will allow for the easy tracking of hydrogen availability and pricing, 
providing a valuable tool for businesses and individuals looking to utilize hydrogen fuel for transportation and energy purposes. 
The platform will also incentivize the production of hydrogen by providing rewards for producers and distributors who meet certain 
efficiency and sustainability standards.

By adding this Hydrogen Availability Management Platform, CURA (XBTX) aims to become a comprehensive platform for sustainable energy 
and healthcare-related services, offering users a range of tools and services to improve their quality of 
life while also promoting environmental sustainability.


Thank you to the Bitcoin developers. 

The Cura project is launched based on the hard work and continuous effort of over 400 Bitcoin developers who made over 14,000 commits 
over the life to date of the Bitcoin project. We are eternally grateful to you for your efforts and diligence in making a secure network 
and for their support of free and open source software development.  
The Cura experiment is made on the foundation you built.


Abstract
----------------
CURA (XBTX) is a decentralized digital currency that offers fast and secure transactions, 
low transaction fees, and staking rewards for users. The Scrypt^2 algorithm ensures a fair and efficient mining process, 
while the decentralized nature of the currency makes it resistant to censorship and government intervention. XBTX has several use cases, 
including peer-to-peer transactions, e-commerce, and a Hydrogen Availability Management Platform that provides a decentralized solution to 
manage the availability and distribution of hydrogen fuel. 
This platform uses smart contracts and blockchain technology to create a transparent and secure system for tracking the production, 
distribution, and consumption of hydrogen. With its potential to become a popular digital currency for peer-to-peer transactions and 
its focus on sustainability, CURA (XBTX) offers users a comprehensive platform for sustainable energy and healthcare-related services.

Let Cura be the final cure for the global financial systems, healthcare and energy supply
