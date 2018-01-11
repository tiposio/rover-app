/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    MQTT Subscribe Task
 *
 * Contributors:
 *    M.Ozcelikors <mozcelikors@gmail.com>, created MQTT Subscribe task 11.12.2017
 *
 */

#include <tasks/mqtt_subscribe_task.h>

#include <unistd.h>
#include <ctime>
#include <libraries/timing/timing.h>
#include <interfaces.h>
#include <pthread.h>

#include <roverapp.h>
#include <roverapi/rover_pahomqtt.hpp>
#include <roverapi/rover_mqttcommand.hpp>

#include <wiringPi.h>

void *MQTT_Subscribe_Task (void * arg)
{
	timing mqtt_subscribe_task_tmr;

	int rt = 0;
	int try_count = 0;
	const int max_tries = 20;

	mqtt_subscribe_task_tmr.setTaskID("MQTTSubscribe");
	mqtt_subscribe_task_tmr.setDeadline(0.1);
	mqtt_subscribe_task_tmr.setPeriod(0.1);

	RoverMQTTCommand rover_mqtt = RoverMQTTCommand (	MQTT_BROKER,//"172.22.167.161",
														MQTT_BROKER_PORT,//1883,
														ROVER_IDENTITY,
														ROVER_MQTT_QOS,
														"rover_mqtt_subscriber");
	RoverControlData_t control_data;

	/* Try to subscribe to the driving topic for a while (max_tries times) */
	do
	{
		if (running_flag.get())
		{
			rt = rover_mqtt.subscribeToDrivingTopic();
			if (rt == 0)
			{
				printf ("Client rover_mqtt_subscriber: Subscription succesful!\n");
				break;
			}
			else
			{
				printf ("Client rover_mqtt_subscriber: Subscription unsuccessful!\n");
			}
			if (try_count >= max_tries - 1)
				break;
			try_count += 1;
			delay (1500);
		}
		else
			break;
	}
	while (rt != 0);

	/* Main thread loop */
	while (running_flag.get())
	{
		mqtt_subscribe_task_tmr.recordStartTime();
		mqtt_subscribe_task_tmr.calculatePreviousSlackTime();

		//Task content starts here -----------------------------------------------

		control_data = rover_mqtt.readFromDrivingTopic();

		/* Override driving-related global variables if data is ready */
		if (control_data.data_ready == 1)
		{
			speed_shared = control_data.speed;
			keycommand_shared = control_data.command;
			driving_mode = control_data.driving_mode;
			printf ("Client rover_mqtt_subscriber: Data received: command=%c speed=%d mode=%d\n",
																  control_data.command,
																  control_data.speed,
																  control_data.driving_mode);

		}


		//Task content ends here -------------------------------------------------

		mqtt_subscribe_task_tmr.recordEndTime();
		mqtt_subscribe_task_tmr.calculateExecutionTime();
		mqtt_subscribe_task_tmr.calculateDeadlineMissPercentage();
		mqtt_subscribe_task_tmr.incrementTotalCycles();
		pthread_mutex_lock(&mqtt_subscribe_task_ti.mutex);
			mqtt_subscribe_task_ti.deadline = mqtt_subscribe_task_tmr.getDeadline();
			mqtt_subscribe_task_ti.deadline_miss_percentage = mqtt_subscribe_task_tmr.getDeadlineMissPercentage();
			mqtt_subscribe_task_ti.execution_time = mqtt_subscribe_task_tmr.getExecutionTime();
			mqtt_subscribe_task_ti.period = mqtt_subscribe_task_tmr.getPeriod();
			mqtt_subscribe_task_ti.prev_slack_time = mqtt_subscribe_task_tmr.getPrevSlackTime();
			mqtt_subscribe_task_ti.task_id = mqtt_subscribe_task_tmr.getTaskID();
			mqtt_subscribe_task_ti.start_time = mqtt_subscribe_task_tmr.getStartTime();
			mqtt_subscribe_task_ti.end_time = mqtt_subscribe_task_tmr.getEndTime();
		pthread_mutex_unlock(&mqtt_subscribe_task_ti.mutex);
		mqtt_subscribe_task_tmr.sleepToMatchPeriod();
	}

	/* the function must return something - NULL will do */
	return NULL;
}
