-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 05-09-2024 a las 17:13:33
-- Versión del servidor: 10.4.32-MariaDB
-- Versión de PHP: 8.0.30

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `sensor_data`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `data`
--

CREATE TABLE `data` (
  `id` int(11) NOT NULL,
  `px` float DEFAULT NULL,
  `py` float DEFAULT NULL,
  `pz` float DEFAULT NULL,
  `roll` float DEFAULT NULL,
  `pitch` float DEFAULT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `data`
--

INSERT INTO `data` (`id`, `px`, `py`, `pz`, `roll`, `pitch`, `timestamp`) VALUES
(1, 0, 0, 0, -3.49, 1.06, '2024-09-05 14:51:51'),
(2, 0, 0, 0, -3.79, 1.59, '2024-09-05 14:51:56'),
(3, 0, 0, 0, -2.61, 0.9, '2024-09-05 14:52:01'),
(4, 0, 0, 0, -2.44, 0.14, '2024-09-05 14:52:06'),
(5, 0, 0, 0, -0.57, -0.61, '2024-09-05 14:52:11'),
(6, 0, 0, 0, 0.52, 0.16, '2024-09-05 14:52:16'),
(7, 0, 0, 0, 1.92, 0.71, '2024-09-05 14:52:21'),
(8, 0, 0, 0, 3.81, 0.94, '2024-09-05 14:52:26'),
(9, 0, 0, 0, 2.55, -1.17, '2024-09-05 14:56:35'),
(10, 0, 0, 0, 4.81, -1.9, '2024-09-05 14:56:40'),
(11, 0, -5034.97, -4730.43, 3.94, -2.64, '2024-09-05 14:56:45'),
(12, -2581.2, -5046050, -4740580, 344.78, -209.14, '2024-09-05 14:56:50'),
(13, -2588600, -5046060000, -4740600000, 104.78, -186.15, '2024-09-05 14:56:55'),
(14, -2588620000, -5046060000000, -4740600000000, 99.09, -182.03, '2024-09-05 14:57:00'),
(15, -2588620000000, -5.04606e15, -4.7406e15, 94.64, -176.49, '2024-09-05 14:57:05'),
(16, -2.58862e15, -5.04606e18, -4.7406e18, 89.66, -171.54, '2024-09-05 14:57:10'),
(17, 0, 0, 0, 3.35, -0.94, '2024-09-05 15:03:53'),
(18, 0, 0, 0, 6.18, -1.4, '2024-09-05 15:03:58'),
(19, 0, 0, 0, 8.72, -1.41, '2024-09-05 15:04:03'),
(20, 0, 0, 0, 12.68, -1.35, '2024-09-05 15:04:08'),
(21, 0, 0, 0, 14.87, -0.78, '2024-09-05 15:04:13'),
(22, 0, 0, 0, 18.44, -0.03, '2024-09-05 15:04:18'),
(23, 0, 0, 0, 16.12, 1.09, '2024-09-05 15:04:23'),
(24, 0, 0, 0, 13.88, 1.33, '2024-09-05 15:04:28'),
(25, 0, 0, 0, -0.2, -0.48, '2024-09-05 15:08:22'),
(26, 0, 0, 0, -1.07, -1, '2024-09-05 15:08:27'),
(27, 0, 0, 0, -1.52, -0.14, '2024-09-05 15:08:31'),
(28, 0, 0, 0, -1.22, -0.14, '2024-09-05 15:08:36'),
(29, 2573, 1104.55, 0, 9.59, -7.03, '2024-09-05 15:08:41'),
(30, 2578140, 1106780, 0, 10.36, -8.32, '2024-09-05 15:08:46'),
(31, 2578140000, 1106780000, 0, 12.39, -10.4, '2024-09-05 15:08:51'),
(32, 2578140000000, 1106780000000, 0, -12.6, -356.94, '2024-09-05 15:08:56');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `data`
--
ALTER TABLE `data`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `data`
--
ALTER TABLE `data`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
