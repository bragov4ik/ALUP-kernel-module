# Task 3 report

Task 2 is already completed, so let's go through the checkboxes of task 3:

### Add dynamic memory allocation to kernel module.

Dynamic memory was added with the help of `kmalloc`. Even though it appears that it doesn't guarantee big memory sizes, it is still dynamic and should be enough for stack of integers.

All previous tests are passing with dynamic memory.

### Add ioctl() call support to change queue size in dynamic (maximum size is not limited, return error on failure).

Done, also a few tests for resize are added.

### Add concurrent access to your module. (protect with lock, use threads, workqueues, everythin is up to you).

Module operations (that require concurrent access) are now guarded with mutex. Mutex is chosen in accordance to [kernel docs](https://docs.kernel.org/locking/mutex-design.html#when-to-use-mutexes); they recommend to use if nothing prevents us from doing it.

Concurrent access is also checked in test with the help of `pthread`s.

## Output of the tests

```console
# ./launch.sh 
stack_device: loading out-of-tree module taints kernel.
Registering character device...
Successfully registered character device with major 248
To talk to the driver, create a dev file with
'mknod /dev/stack_file_kek_chrdev0 c 248 0'.
Try various minor numbers. Try to cat and echo to
the device file.
Remove the device file and module when done.
Testing /dev/stack_file_kek_chrdev0
No data in stack left.
Preparing the file..Finished clearing file (No more data left)
Getting device major number for stack_file_kek_chrdev.
Found number: 248.
Received ioctl 1074067457!
Resizing the stack to 256
File should be readyRunning test One element
Writing '-31337' (7 bytes)
Read '-31337'
Test passed!

Running test Negative element
Writing '-31337' (7 bytes)
Read '-31337'
Test passed!

Running test Ten elements
Writing '31337' (6 bytes)
Writing '123' (4 bytes)
Writing '456' (4 bytes)
Writing '1' (2 bytes)
Writing '2' (2 bytes)
Writing '3' (2 bytes)
Writing '-3' (3 bytes)
Writing '-2' (3 bytes)
Writing '-1' (3 bytes)
Writing '0' (2 bytes)
Read '0'
Read '-1'
Read '-2'
Read '-3'
Read '3'
Read '2'
Read '1'
Read '456'
Read '123'
Read '31337'
Test passed!

Running test Min max
Writing '2147483647' (11 bytes)
Writing '-2147483648' (12 bytes)
Read '-2147483648'
Read '2147483647'
Test passed!

Running test 256 numbers
Writing '-128' (5 bytes)
Writing '-127' (5 bytes)
Writing '-126' (5 bytes)
Writing '-125' (5 bytes)
Writing '-124' (5 bytes)
Writing '-123' (5 bytes)
Writing '-122' (5 bytes)
Writing '-121' (5 bytes)
Writing '-120' (5 bytes)
Writing '-119' (5 bytes)
Writing '-118' (5 bytes)
Writing '-117' (5 bytes)
Writing '-116' (5 bytes)
Writing '-115' (5 bytes)
Writing '-114' (5 bytes)
Writing '-113' (5 bytes)
Writing '-112' (5 bytes)
Writing '-111' (5 bytes)
Writing '-110' (5 bytes)
Writing '-109' (5 bytes)
Writing '-108' (5 bytes)
Writing '-107' (5 bytes)
Writing '-106' (5 bytes)
Writing '-105' (5 bytes)
Writing '-104' (5 bytes)
Writing '-103' (5 bytes)
Writing '-102' (5 bytes)
Writing '-101' (5 bytes)
Writing '-100' (5 bytes)
Writing '-99' (4 bytes)
Writing '-98' (4 bytes)
Writing '-97' (4 bytes)
Writing '-96' (4 bytes)
Writing '-95' (4 bytes)
Writing '-94' (4 bytes)
Writing '-93' (4 bytes)
Writing '-92' (4 bytes)
Writing '-91' (4 bytes)
Writing '-90' (4 bytes)
Writing '-89' (4 bytes)
Writing '-88' (4 bytes)
Writing '-87' (4 bytes)
Writing '-86' (4 bytes)
Writing '-85' (4 bytes)
Writing '-84' (4 bytes)
Writing '-83' (4 bytes)
Writing '-82' (4 bytes)
Writing '-81' (4 bytes)
Writing '-80' (4 bytes)
Writing '-79' (4 bytes)
Writing '-78' (4 bytes)
Writing '-77' (4 bytes)
Writing '-76' (4 bytes)
Writing '-75' (4 bytes)
Writing '-74' (4 bytes)
Writing '-73' (4 bytes)
Writing '-72' (4 bytes)
Writing '-71' (4 bytes)
Writing '-70' (4 bytes)
Writing '-69' (4 bytes)
Writing '-68' (4 bytes)
Writing '-67' (4 bytes)
Writing '-66' (4 bytes)
Writing '-65' (4 bytes)
Writing '-64' (4 bytes)
Writing '-63' (4 bytes)
Writing '-62' (4 bytes)
Writing '-61' (4 bytes)
Writing '-60' (4 bytes)
Writing '-59' (4 bytes)
Writing '-58' (4 bytes)
Writing '-57' (4 bytes)
Writing '-56' (4 bytes)
Writing '-55' (4 bytes)
Writing '-54' (4 bytes)
Writing '-53' (4 bytes)
Writing '-52' (4 bytes)
Writing '-51' (4 bytes)
Writing '-50' (4 bytes)
Writing '-49' (4 bytes)
Writing '-48' (4 bytes)
Writing '-47' (4 bytes)
Writing '-46' (4 bytes)
Writing '-45' (4 bytes)
Writing '-44' (4 bytes)
Writing '-43' (4 bytes)
Writing '-42' (4 bytes)
Writing '-41' (4 bytes)
Writing '-40' (4 bytes)
Writing '-39' (4 bytes)
Writing '-38' (4 bytes)
Writing '-37' (4 bytes)
Writing '-36' (4 bytes)
Writing '-35' (4 bytes)
Writing '-34' (4 bytes)
Writing '-33' (4 bytes)
Writing '-32' (4 bytes)
Writing '-31' (4 bytes)
Writing '-30' (4 bytes)
Writing '-29' (4 bytes)
Writing '-28' (4 bytes)
Writing '-27' (4 bytes)
Writing '-26' (4 bytes)
Writing '-25' (4 bytes)
Writing '-24' (4 bytes)
Writing '-23' (4 bytes)
Writing '-22' (4 bytes)
Writing '-21' (4 bytes)
Writing '-20' (4 bytes)
Writing '-19' (4 bytes)
Writing '-18' (4 bytes)
Writing '-17' (4 bytes)
Writing '-16' (4 bytes)
Writing '-15' (4 bytes)
Writing '-14' (4 bytes)
Writing '-13' (4 bytes)
Writing '-12' (4 bytes)
Writing '-11' (4 bytes)
Writing '-10' (4 bytes)
Writing '-9' (3 bytes)
Writing '-8' (3 bytes)
Writing '-7' (3 bytes)
Writing '-6' (3 bytes)
Writing '-5' (3 bytes)
Writing '-4' (3 bytes)
Writing '-3' (3 bytes)
Writing '-2' (3 bytes)
Writing '-1' (3 bytes)
Writing '0' (2 bytes)
Writing '1' (2 bytes)
Writing '2' (2 bytes)
Writing '3' (2 bytes)
Writing '4' (2 bytes)
Writing '5' (2 bytes)
Writing '6' (2 bytes)
Writing '7' (2 bytes)
Writing '8' (2 bytes)
Writing '9' (2 bytes)
Writing '10' (3 bytes)
Writing '11' (3 bytes)
Writing '12' (3 bytes)
Writing '13' (3 bytes)
Writing '14' (3 bytes)
Writing '15' (3 bytes)
Writing '16' (3 bytes)
Writing '17' (3 bytes)
Writing '18' (3 bytes)
Writing '19' (3 bytes)
Writing '20' (3 bytes)
Writing '21' (3 bytes)
Writing '22' (3 bytes)
Writing '23' (3 bytes)
Writing '24' (3 bytes)
Writing '25' (3 bytes)
Writing '26' (3 bytes)
Writing '27' (3 bytes)
Writing '28' (3 bytes)
Writing '29' (3 bytes)
Writing '30' (3 bytes)
Writing '31' (3 bytes)
Writing '32' (3 bytes)
Writing '33' (3 bytes)
Writing '34' (3 bytes)
Writing '35' (3 bytes)
Writing '36' (3 bytes)
Writing '37' (3 bytes)
Writing '38' (3 bytes)
Writing '39' (3 bytes)
Writing '40' (3 bytes)
Writing '41' (3 bytes)
Writing '42' (3 bytes)
Writing '43' (3 bytes)
Writing '44' (3 bytes)
Writing '45' (3 bytes)
Writing '46' (3 bytes)
Writing '47' (3 bytes)
Writing '48' (3 bytes)
Writing '49' (3 bytes)
Writing '50' (3 bytes)
Writing '51' (3 bytes)
Writing '52' (3 bytes)
Writing '53' (3 bytes)
Writing '54' (3 bytes)
Writing '55' (3 bytes)
Writing '56' (3 bytes)
Writing '57' (3 bytes)
Writing '58' (3 bytes)
Writing '59' (3 bytes)
Writing '60' (3 bytes)
Writing '61' (3 bytes)
Writing '62' (3 bytes)
Writing '63' (3 bytes)
Writing '64' (3 bytes)
Writing '65' (3 bytes)
Writing '66' (3 bytes)
Writing '67' (3 bytes)
Writing '68' (3 bytes)
Writing '69' (3 bytes)
Writing '70' (3 bytes)
Writing '71' (3 bytes)
Writing '72' (3 bytes)
Writing '73' (3 bytes)
Writing '74' (3 bytes)
Writing '75' (3 bytes)
Writing '76' (3 bytes)
Writing '77' (3 bytes)
Writing '78' (3 bytes)
Writing '79' (3 bytes)
Writing '80' (3 bytes)
Writing '81' (3 bytes)
Writing '82' (3 bytes)
Writing '83' (3 bytes)
Writing '84' (3 bytes)
Writing '85' (3 bytes)
Writing '86' (3 bytes)
Writing '87' (3 bytes)
Writing '88' (3 bytes)
Writing '89' (3 bytes)
Writing '90' (3 bytes)
Writing '91' (3 bytes)
Writing '92' (3 bytes)
Writing '93' (3 bytes)
Writing '94' (3 bytes)
Writing '95' (3 bytes)
Writing '96' (3 bytes)
Writing '97' (3 bytes)
Writing '98' (3 bytes)
Writing '99' (3 bytes)
Writing '100' (4 bytes)
Writing '101' (4 bytes)
Writing '102' (4 bytes)
Writing '103' (4 bytes)
Writing '104' (4 bytes)
Writing '105' (4 bytes)
Writing '106' (4 bytes)
Writing '107' (4 bytes)
Writing '108' (4 bytes)
Writing '109' (4 bytes)
Writing '110' (4 bytes)
Writing '111' (4 bytes)
Writing '112' (4 bytes)
Writing '113' (4 bytes)
Writing '114' (4 bytes)
Writing '115' (4 bytes)
Writing '116' (4 bytes)
Writing '117' (4 bytes)
Writing '118' (4 bytes)
Writing '119' (4 bytes)
Writing '120' (4 bytes)
Writing '121' (4 bytes)
Writing '122' (4 bytes)
Writing '123' (4 bytes)
Writing '124' (4 bytes)
Writing '125' (4 bytes)
Writing '126' (4 bytes)
Writing '127' (4 bytes)
Read '127'
Read '126'
Read '125'
Read '124'
Read '123'
Read '122'
Read '121'
Read '120'
Read '119'
Read '118'
Read '117'
Read '116'
Read '115'
Read '114'
Read '113'
Read '112'
Read '111'
Read '110'
Read '109'
Read '108'
Read '107'
Read '106'
Read '105'
Read '104'
Read '103'
Read '102'
Read '101'
Read '100'
Read '99'
Read '98'
Read '97'
Read '96'
Read '95'
Read '94'
Read '93'
Read '92'
Read '91'
Read '90'
Read '89'
Read '88'
Read '87'
Read '86'
Read '85'
Read '84'
Read '83'
Read '82'
Read '81'
Read '80'
Read '79'
Read '78'
Read '77'
Read '76'
Read '75'
Read '74'
Read '73'
Read '72'
Read '71'
Read '70'
Read '69'
Read '68'
Read '67'
Read '66'
Read '65'
Read '64'
Read '63'
Read '62'
Read '61'
Read '60'
Read '59'
Read '58'
Read '57'
Read '56'
Read '55'
Read '54'
Read '53'
Read '52'
Read '51'
Read '50'
Read '49'
Read '48'
Read '47'
Read '46'
Read '45'
Read '44'
Read '43'
Read '42'
Read '41'
Read '40'
Read '39'
Read '38'
Read '37'
Read '36'
Read '35'
Read '34'
Read '33'
Read '32'
Read '31'
Read '30'
Read '29'
Read '28'
Read '27'
Read '26'
Read '25'
Read '24'
Read '23'
Read '22'
Read '21'
Read '20'
Read '19'
Read '18'
Read '17'
Read '16'
Read '15'
Read '14'
Read '13'
Read '12'
Read '11'
Read '10'
Read '9'
Read '8'
Read '7'
Read '6'
Read '5'
Read '4'
Read '3'
Read '2'
Read '1'
Read '0'
Read '-1'
Read '-2'
Read '-3'
Read '-4'
Read '-5'
Read '-6'
Read '-7'
Read '-8'
Read '-9'
Read '-10'
Read '-11'
Read '-12'
Read '-13'
Read '-14'
Read '-15'
Read '-16'
Read '-17'
Read '-18'
Read '-19'
Read '-20'
Read '-21'
Read '-22'
Read '-23'
Read '-24'
Read '-25'
Read '-26'
Read '-27'
Read '-28'
Read '-29'
Read '-30'
Read '-31'
Read '-32'
Read '-33'
Read '-34'
Read '-35'
Read '-36'
Read '-37'
Read '-38'
Read '-39'
Read '-40'
Read '-41'
Read '-42'
Read '-43'
Read '-44'
Read '-45'
Read '-46'
Read '-47'
Read '-48'
Read '-49'
Read '-50'
Read '-51'
Read '-52'
Read '-53'
Read '-54'
Read '-55'
Read '-56'
Read '-57'
Read '-58'
Read '-59'
Read '-60'
Read '-61'
Read '-62'
Read '-63'
Read '-64'
Read '-65'
Read '-66'
Read '-67'
Read '-68'
Read '-69'
Read '-70'
Read '-71'
Read '-72'
Read '-73'
Read '-74'
Read '-75'
Read '-76'
Read '-77'
Read '-78'
Read '-79'
Read '-80'
Read '-81'
Read '-82'
Read '-83'
Read '-84'
Read '-85'
Read '-86'
Read '-87'
Read '-88'
Read '-89'
Read '-90'
Read '-91'
Read '-92'
Read '-93'
Read '-94'
Read '-95'
Read '-96'
Read '-97'
Read '-98'
Read '-99'
Read '-100'
Read '-101'
Read '-102'
Read '-103'
Read '-104'
Read '-105'
Read '-106'
Read '-107'
Read '-108'
Read '-109'
Read '-110'
Read '-111'
Read '-112'
Read '-113'
Read '-114'
Read '-115'
Read '-116'
Read '-117'
Read '-118'
Read '-119'
Read '-120'
Read '-121'
Read '-122'
Read '-123'
Read '-124'
Read '-125'
Read '-126'
Read '-127'
Read '-128'
Test passed!

Running test Resize
Getting device major number for stack_file_kek_chrdev.
Found number: 248.
Received ioctl 1074067457!
Resizing the stack to 10
Writing '31337' (6 bytes)
Writing '123' (4 bytes)
Writing '456' (4 bytes)
Writing '1' (2 bytes)
Writing '2' (2 bytes)
Writing '3' (2 bytes)
Writing '-3' (3 bytes)
Writing '-2' (3 bytes)
Writing '-1' (3 bytes)
Writing '0' (2 bytes)
Read '0'
Read '-1'
Read '-2'
Read '-3'
Read '3'
Read '2'
Read '1'
Read '456'
Read '123'
Read '31337'
Getting device major number for stack_file_kek_chrdev.
Found number: 248.
Received ioctl 1074067457!
Resizing the stack to 9
Writing '31337' (6 bytes)
Writing '123' (4 bytes)
Writing '456' (4 bytes)
Writing '1' (2 bytes)
Writing '2' (2 bytes)
Writing '3' (2 bytes)
Writing '-3' (3 bytes)
Writing '-2' (3 bytes)
Writing '-1' (3 bytes)
Writing '0' (2 bytes)
Stack is full.
No data in stack left.
Finished clearing file (No more data left)
Getting device major number for stack_file_kek_chrdev.
Found number: 248.
Received ioctl 1074067457!
Resizing the stack to 256
Test passed!

Running test Concurrency
Writing '1' (2 bytes)
Writing '10' (3 bytes)
Writing '20' (3 bytes)
Writing '30' (3 bytes)
Writing '40' (3 bytes)
Writing '50' (3 bytes)
Writing '60' (3 bytes)
Writing '70' (3 bytes)
Writing '80' (3 bytes)
Writing '90' (3 bytes)
Read '90'
Read '80'
Read '70'
Read '60'
Read '50'
Writing '2' (2 bytes)
Writing '3' (2 bytes)
Writing '4' (2 bytes)
Writing '5' (2 bytes)
Writing '6' (2 bytes)
Writing '7' (2 bytes)
Writing '8' (2 bytes)
Writing '9' (2 bytes)
Read '9'
Read '8'
Read '7'
Read '6'
Read '5'
Read '4'
Read '3'
Read '2'
Read '40'
Read '30'
Read '20'
Read '10'
Read '1'
Thread 1 returned 0 (errno Success)
Thread 2 returned 0 (errno Success)
Test passed!

No data in stack left.
Finished clearing file (No more data left)

Passed 7/7 tests
```
