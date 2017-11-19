
-- CREATE DATABASE bankBase;
use bankBase;

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE `salts` (
  `id` int(128) NOT NULL,
  `user` varchar(50) NOT NULL,
  `salt` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


INSERT INTO `salts` (`id`, `user`, `salt`) VALUES
(1, 'pandemic', 'E1F53135E559C253');

ALTER TABLE `salts`
  ADD PRIMARY KEY (`id`);


CREATE TABLE `clients` (
  `id` int(128) PRIMARY KEY NOT NULL,
  `user` varchar(50) NOT NULL,
  `password` varchar(128) NOT NULL,
  `name` varchar(128) NOT NULL,
  `number` varchar(128) NOT NULL,
  `balance` float(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


INSERT INTO `clients` (`id`, `user`, `password`, `name`, `number`, `balance`) VALUES
(1, 'pandemic', 'b4db1fcde544c447713ef355deff5fbb82a287c4', 'Jan Kowalski', '1234567891012345', 73672.12);

-- drop table history;

CREATE TABLE `history` (
  `id` int(128) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `recipient` int(11) NOT NULL,
  `sender` varchar(50) NOT NULL,
  `value` varchar(128) NOT NULL,
  `execution_date` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
-- --
ALTER TABLE `clients`
  MODIFY `id` int(128) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

INSERT INTO `salts` (`id`, `user`, `salt`) VALUES
(2, 'etude', '84B03D034B409D4E');

INSERT INTO `clients` (`user`, `password`, `name`, `number`, `balance`) VALUES
('etude', 'e53b0bfde22dac5239e78488b0f510120a44e9f7', 'Anna Zaradna', '9876543211012345', 736.76);

INSERT INTO `history` (`recipient`, `sender`, `value`, `execution_date`) VALUES
('1234567891012345', '9876543211012345', 145, now());


