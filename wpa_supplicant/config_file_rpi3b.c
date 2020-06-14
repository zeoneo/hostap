/*
 * WPA Supplicant / Configuration backend: empty starting point
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file implements dummy example of a configuration backend. None of the
 * functions are actually implemented so this can be used as a simple
 * compilation test or a starting point for a new configuration backend.
 */

#include "includes.h"

#include "common.h"
#include "config.h"
#include "base64.h"

static char *net_ssid = "BlackCat";
static char *net_password = "RatKillerCatBlack567";

struct wpa_config * wpa_config_read(const char *name)
{
	struct wpa_config *config;

	config = wpa_config_alloc_empty(NULL, NULL);
	if (config == NULL)
		return NULL;
	
	struct wpa_ssid *ssid = os_malloc(sizeof(struct wpa_ssid));

	if(ssid == NULL) {
		return NULL;
	}
	ssid->ssid = net_ssid;
	ssid->ssid_len = os_strlen(net_ssid);
	os_strlcpy(ssid->psk, net_password, os_strlen(net_password));
	ssid->proto = WPA_PROTO_WPA;
	ssid->key_mgmt = WPA_KEY_MGMT_PSK;
	ssid->next = 0;
	ssid->pnext = 0;
	
	config->ssid = ssid;
	config->country[0] = 'I';
	config->country[1] = 'N';
	return config;
}


int wpa_config_write(const char *name, struct wpa_config *config)
{
	struct wpa_ssid *ssid;
	struct wpa_config_blob *blob;

	wpa_printf(MSG_DEBUG, "Writing configuration file '%s'", name);

	/* TODO: write global config parameters */


	for (ssid = config->ssid; ssid; ssid = ssid->next) {
		/* TODO: write networks */
	}

	for (blob = config->blobs; blob; blob = blob->next) {
		/* TODO: write blobs */
	}

	return 0;
}
