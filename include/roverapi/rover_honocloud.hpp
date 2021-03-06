/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    Rover Hono Cloud API - Interfaces for Rover Hono cloud application development - Header file
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created API 17.11.2017
 *
 */

#ifndef API_ROVER_HONO_CLOUD_HPP_
#define API_ROVER_HONO_CLOUD_HPP_

#include <roverapi/rover_cloud.hpp>

namespace rover
{
	/**
	 * @brief Contains the member functions to connect and send data to Eclipse Hono instance using several parameters
	 * such as host name, port, tenant name, user name, and password. This class wraps hono_interaction library for Rover-specific interactions.
	 */
	class RoverHonoCloud : public RoverCloud
	{
		private:
			/**
			 * @brief Host name used for connecting to the Eclipse Hono using REST API
			 */
			char * HOST_NAME;
			/**
			 * @brief Port used for connecting to the Eclipse Hono using REST API
			 */
			int PORT;
			/**
			 * @brief Port used for registering a device to the Eclipse Hono using REST API
			 */
			mutable int REGISTRATION_PORT;
			/**
			 * @brief Tenant name used for connecting to the Eclipse Hono using REST API
			 */
			mutable char * TENANT_NAME;

			/**
			 * @brief Checks private attributes and gives an error message and returns 0 if they're invalid.
			 */
			int attributeErrorCheck (void) const;

		public:
			/**
			 * @brief (default) Constructor for the RoverHonoCloud
			 */
			explicit RoverHonoCloud();

			/**
			 * @brief (assigning) Constructor for the RoverHonoCloud
			 * @param host_name
			 * @param registration_port
			 * @param port
			 * @param tenant
			 */
			explicit RoverHonoCloud(char * host_name, const int port, const int registration_port, char * tenant_name);

			/**
			 * @brief Sets private attribute HOST_NAME
			 * @param host_name
			 */
			void setHostName (char * host_name);

			/**
			 * @brief Sets private attribute PORT
			 * @param port
			 */
			void setPort (const int port);

			/**
			 * @brief Sets private attribute TENANT_NAME
			 * @param tenant
			 */
			void setTenantName (char * tenant) const;

			/**
			 * @brief Sets up a hono connection using host name, port, and tenant name
			 * @param host_name
			 * @param port
			 * @param tenant
			 */
			void setHono (char * host_name, const int port, char * tenant);

			/**
			 * @brief Retrieves private attribute HOST_NAME
			 */
			char * getHostName (void) const;

			/**
			 * @brief Retrieves private attribute PORT
			 */
			int getPort (void) const;

			/**
			 * @brief Retrieves private attribute TENANT_NAME
			 */
			char * getTenantName (void) const;

			/**
			 * @brief Sets the default REGISTRATION_PORT variable
			 * @param port (int) to be set as REGISTRATION_PORT
			 * @return void
			 */
			void setRegistrationPort (const int port) const;

			/**
			 * @brief Retrieves private attribute REGISTRATION_PORT
			 * @return Private attribute REGISTRATION_PORT
			 */
			int getRegistrationPort (void) const;

			/**
			 * @brief Constructs curl command to register a device to Hono instance given device ID using REST API
			 * @param device_id
			 * @return status code (int) is returned. 1 indicates successful operation and 0 indicates unsuccessful operation.
			 */
			int registerDevice (char * device_id);

			/**
			 * @brief Constructs curl command to send telemetry data to Eclipse Hono using REST API
			 * @return status code (int) is returned. 1 indicates successful operation and 0 indicates unsuccessful operation.
			 */
			int sendTelemetry (char * device_id, char * user, char * password, char * field, double value);

			/**
			 * @brief Constructs curl command to send event data to Eclipse Hono using REST API
			 * @return status code (int) is returned. 1 indicates successful operation and 0 indicates unsuccessful operation.
			 */
			int sendEvent (char * device_id, char * user, char * password, char * field, double value);

			/**
			 * @brief Retrieves Hono cloud status by constructing a dummy REST API telemetry sending command to Eclipse Hono
			 * @return status code (int) is returned. 1 indicates successful operation and 0 indicates unsuccessful operation.
			 */
			int getHonoStatus (char * device_id, char * user, char * password);
	};
}


#endif /* API_ROVER_HONO_CLOUD_HPP_ */
