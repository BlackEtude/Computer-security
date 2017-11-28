
-- CREATE DATABASE bankBase;
use bankBase;

-- SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
-- SET time_zone = "+00:00";

-- CREATE TABLE `salts` (
--   `id` int(128) NOT NULL,
--   `user` varchar(50) NOT NULL,
--   `salt` varchar(128) NOT NULL
-- ) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- INSERT INTO `salts` (`id`, `user`, `salt`) VALUES
-- (1, 'pandemic', 'E1F53135E559C253');

-- ALTER TABLE `salts`
--   ADD PRIMARY KEY (`id`);


-- CREATE TABLE `clients` (
--   `id` int(128) PRIMARY KEY NOT NULL,
--   `user` varchar(50) NOT NULL,
--   `password` varchar(128) NOT NULL,
--   `name` varchar(128) NOT NULL,
--   `number` varchar(128) NOT NULL,
--   `balance` float(11) NOT NULL
-- ) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- INSERT INTO `clients` (`id`, `user`, `password`, `name`, `number`, `balance`) VALUES
-- (1, 'pandemic', 'b4db1fcde544c447713ef355deff5fbb82a287c4', 'Jan Kowalski', '1234567891012345', 73672.12);

-- drop table history;

-- CREATE TABLE `history` (
--   `id` int(128) NOT NULL PRIMARY KEY AUTO_INCREMENT,
--   `title` varchar(128) NOT NULL,
--   `recipient` varchar(128) NOT NULL,
--   `sender` varchar(128) NOT NULL,
--   `value` varchar(128) NOT NULL,
--   `execution_date` datetime NOT NULL
-- ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
-- --
-- ALTER TABLE `clients`
--   MODIFY `id` int(128) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

-- INSERT INTO `salts` (`id`, `user`, `salt`) VALUES
-- (3, 'root', 'ED3TGyJrzi6VVjQp');

-- UPDATE clients
-- SET number = '1111111111111111'
-- WHERE clients.user='root'; 

-- INSERT INTO `clients` (`user`, `password`, `name`, `number`, `balance`) VALUES
-- ('root', 'e53b0bfde22dac5239e78488b0f510120a44e9f7', 'Adam Niezbędny', '0000000000000001', 73608.67);

-- INSERT INTO `history` (`recipient`, `sender`, `value`, `execution_date`) VALUES
-- ('1234567891012345', '9876543211012345', 432, now());


-- CREATE TABLE `admin` (
--   `user` varchar(50) NOT NULL PRIMARY KEY,
--   `password` varchar(128) NOT NULL
-- ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- INSERT INTO `admin` (`user`, `password`) VALUES
-- ('superadmin', '2ca78e065af54a64061af0b5382288621fd141dc ');

-- CREATE TABLE `pending` (
--   `id` int(128) NOT NULL PRIMARY KEY AUTO_INCREMENT,
--   `title` varchar(128) NOT NULL,
--   `recipient` varchar(50) NOT NULL,
--   `sender` varchar(50) NOT NULL,
--   `value` varchar(50) NOT NULL,
--   `execution_date` datetime NOT NULL
-- ) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- CREATE TRIGGER historyTrigger
-- AFTER DELETE ON pending FOR EACH ROW
--   INSERT INTO history (recipient, sender, value, execution_date, title) 
--   VALUES (OLD.recipient, OLD.sender, OLD.value, OLD.execution_date, OLD.title)
