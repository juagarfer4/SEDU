import { Template } from 'meteor/templating';
import { ReactiveVar } from 'meteor/reactive-var';
import { HTTP } from 'meteor/http';

import { Chart } from 'chart.js';

import { Muestras } from '../imports/api/muestras';
import { Session } from 'meteor/session';

import { _ } from 'underscore';

import './main.html';


function creaGraficaFitnGrow() {
  data = Session.get("muestras.fitngrow");
  console.log(data);
  data = _.sortBy(data, function (o) { return -o.entry_id });
  data = data.slice(0, 10);

  data.reverse();


  times = _.pluck(data, "created_at");
  times = _.map(times, function (s) { return s.substr(11, 5) })
  temperatures = _.pluck(data, "field1");
  humidities = _.pluck(data, "field2");
  luminosities = _.pluck(data, "field3");


  var ctx = document.getElementById('tChartFitnGrow').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Temperatura",
        backgroundColor: '#33adff',
        borderColor: '#33adff',
        data: temperatures,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });

  var ctx = document.getElementById('hChartFitnGrow').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Humedad",
        backgroundColor: '#1aff1a',
        borderColor: '#1aff1a',
        data: humidities,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });

  var ctx = document.getElementById('lChartFitnGrow').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Luminosidad",
        backgroundColor: '#ff4d4d',
        borderColor: '#ff4d4d',
        data: luminosities,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });
}

function creaGraficaWordstoJoin() {
  data = Session.get("muestras.wordstojoin");
  console.log(data);
  data = _.sortBy(data, function (o) { return -o.entry_id });
  data = data.slice(0, 10);

  data.reverse();


  times = _.pluck(data, "created_at");
  times = _.map(times, function (s) { return s.substr(11, 5) })
  humidities = _.pluck(data, "field1");
  temperatures = _.pluck(data, "field2");
  resistances = _.pluck(data, "field3");


  var ctx = document.getElementById('tChartWordstoJoin').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Humedad",
        backgroundColor: '#33adff',
        borderColor: '#33adff',
        data: humidities,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });

  var ctx = document.getElementById('hChartWordstoJoin').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Temperatura",
        backgroundColor: '#1aff1a',
        borderColor: '#1aff1a',
        data: temperatures,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });

  var ctx = document.getElementById('lChartWordstoJoin').getContext('2d');
  var chart = new Chart(ctx, {
    // The type of chart we want to create
    type: 'line',

    // The data for our dataset
    data: {
      labels: times,
      datasets: [{
        label: "Resistencia",
        backgroundColor: '#ff4d4d',
        borderColor: '#ff4d4d',
        data: resistances,
        fill: false
      }]
    },

    // Configuration options go here
    options: {}
  });
}

Template.hello.onCreated(function helloOnCreated() {
  // counter starts at 0
  this.counter = new ReactiveVar(0);
  Session.set("muestras.fitngrow", null);
  Session.set("muestras.wordstojoin", null);
  Meteor.call("muestras.get.fitngrow", { results: 20 }, (err, res) => {
    Session.set("muestras.fitngrow", res);
    creaGraficaFitnGrow();
  });
  Meteor.call("muestras.get.wordstojoin", { results: 20 }, (err, res) => {
    Session.set("muestras.wordstojoin", res);
    creaGraficaWordstoJoin();
  });
});

Template.hello.onRendered(function () {
  document.getElementById('fitngrowsection').style.display = "block";
  document.getElementById('wordstojoinsection').style.display = "none";
})

Template.hello.helpers({
  counter() {
    return Template.instance().counter.get();
  },
  muestrasFitnGrow() {
    // return Muestras.find({}, {sort: {D: -1}, limit: 50})
    data = ReactiveMethod.call("muestras.get.fitngrow", { results: 20 });
    data = _.sortBy(data, function (o) { return -o.entry_id });
    return data;
  },
  muestrasWordstoJoin() {
    // return Muestras.find({}, {sort: {D: -1}, limit: 50})
    data = ReactiveMethod.call("muestras.get.wordstojoin", { results: 20 });
    data = _.sortBy(data, function (o) { return -o.entry_id });
    return data;
  },
  dateformat(s) {
    // return Muestras.find({}, {sort: {D: -1}, limit: 50})
    return s.replace('T', ' ').replace('Z', ' ');
  },
});

Template.hello.events({
  'click button'(event, instance) {
    // increment the counter when button is clicked
    instance.counter.set(instance.counter.get() + 1);
  },
  'click #buttonfitngrow'(event, instance) {
    // increment the counter when button is clicked
    document.getElementById('fitngrowsection').style.display = "block";
    document.getElementById('wordstojoinsection').style.display = "none";
  },
  'click #buttonwordstojoin'(event, instance) {
    // increment the counter when button is clicked
    document.getElementById('fitngrowsection').style.display = "none";
    document.getElementById('wordstojoinsection').style.display = "block";
  },
});
